/***************************************************************
 * hilightTool.cpp
 * @Author:      Jonathan Verner (jonathan.verner@matfyz.cz)
 * @License:     GPL v2.0 or later
 * @Created:     2008-11-07.
 * @Last Change: 2008-11-07.
 * @Revision:    0.0
 * Description:
 * Usage:
 * TODO:
 *CHANGES:
 ***************************************************************/


#include "hilightTool.h"
#include "pageView.h" /* needed for viewEvent */
#include "toolBox.h"
#include "pdfScene.h"
#include "pdfUtil.h"
#include "propertyTab.h"

#include <QtCore/QDebug>
#include <QtGui/QIcon>
#include <QtGui/QStackedWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QTabWidget>

#include <poppler-qt4.h>

using namespace Poppler;

QIcon hilightTool::icon;



hilightTool::hilightTool( pdfScene *Scene, toolBox *ToolBar, QStackedWidget *EditArea):
	abstractTool( Scene, ToolBar, EditArea ), editingHilight(false)
{
  icon = QIcon::fromTheme("format-text-underline");
  setToolName( "Hilight Tool" );
  toolBar->addTool( icon, this );
}


void hilightTool::newActionEvent( const QPointF *ScenePos ) {
  hilightAnnotation *hi = new hilightAnnotation(this);
  scene->placeAnnotation( hi, ScenePos );
  hi->setZValue( 9 );
  qDebug() << "Editing item";
  editAnnotationExtent( hi );
}

void hilightTool::updateCurrentAnnotation( QPointF ScenePos ) { 
  hilightAnnotation *annot = dynamic_cast<hilightAnnotation*>(currentEditItem); 
  Q_ASSERT( annot );
  QPointF from = annot->scenePos();
  QPointF to = ScenePos;
  annot->updateSelection( scene->selectText(  from, to ) );
}

bool hilightTool::acceptEventsFor( QGraphicsItem *item ) {  
  return dynamic_cast<hilightAnnotation*>( item );
}

void hilightTool::editItem( abstractAnnotation *item ) { 
  qDebug() << "hilight::editItem";
  if ( currentEditItem == item ) { 
    currentEditItem = NULL;
    editArea->hide();
    setTeXToolTip( item );
  } else {
    currentEditItem = item;
    propertyEdit->setAuthor( item->getAuthor() );
    editAnnotationText();
  }
}

void hilightTool::editAnnotationExtent( abstractAnnotation *item ) { 
   if ( currentEditItem == item ) { 
     currentEditItem = NULL;
     editArea->hide();
     setTeXToolTip( item );
   } else {
     currentEditItem = item;
     editingHilight = true;
   }
}

void hilightTool::editAnnotationText() { 
   editingHilight = false;
   editArea->setCurrentWidget( editor );
   editArea->show();
   contentEdit->setText( currentEditItem->getContent() );
   contentEdit->setFocus();
   editor->setCurrentIndex( 0 );
}
void hilightTool::finishEditing() { 
  if ( dynamic_cast<hilightAnnotation*>(currentEditItem) ) { 
    editArea->hide();
    setTeXToolTip( currentEditItem );
    currentEditItem = NULL;
  }
}
	
bool hilightTool::handleEvent( viewEvent *ev ) { 
  hilightAnnotation *annot;
  if ( ev->type() == viewEvent::VE_MOUSE_PRESS && ( ev->btnCaused() == Qt::LeftButton ) ) {
    if ( annot = dynamic_cast<hilightAnnotation*>(ev->item()) ) { 
      editAnnotationExtent( annot );
    } else {
      QPointF pos = ev->scenePos();
      newActionEvent( &pos );
    }
    return true;
  } else if ( ev->type() == viewEvent::VE_MOUSE_RELEASE && ( ev->btnCaused() == Qt::LeftButton ) ) { 
    if ( ! (annot=dynamic_cast<hilightAnnotation*>(currentEditItem)) ) return false;
    if ( ev->isClick() ) {
      if ( editingHilight ) editAnnotationText();
      else finishEditing();
      return true;
    }
    updateCurrentAnnotation( ev->scenePos() );
    editAnnotationText();
    return true;
  } else if ( ev->type() == viewEvent::VE_MOUSE_MOVE && (annot=dynamic_cast<hilightAnnotation*>(currentEditItem)) ) {
    if ( editingHilight ) {
      updateCurrentAnnotation( ev->scenePos() );
      if ( ! (ev->btnState() & Qt::LeftButton) ) { // Missed a mouse release, end editing annotation
        qDebug() << "WARNING MISSED MOUSE RELEASE EVENT!!!";
        editAnnotationText();
      }
    } else finishEditing();
    return true;  
  } else return abstractTool::handleEvent( ev );
  return false;
}

abstractAnnotation *hilightTool::processAnnotation( PoDoFo::PdfAnnotation *annotation, pdfCoords *transform ) {
  if ( ! hilightAnnotation::isA( annotation ) ) return NULL;
  return new hilightAnnotation( this, annotation, transform );
}


void hilightAnnotation::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget ) {
  QPointF myPos = pos();
  foreach( QRectF box, hBoxes ) { 
    painter->fillRect( box, QColor( 255, 255, 0, 156 ) );
  }
}



QPainterPath hilightAnnotation::shape() const { 
  return exactShape;
}

void hilightAnnotation::updateSelection( QList<TextBox*> newSelection ) { 
  QPainterPath tmp;
  QRectF br;
  update();
  prepareGeometryChange();
  hBoxes.clear();
  foreach( TextBox *box, newSelection ) { 
    br = mapFromParent( box->boundingBox() ).boundingRect();
    hBoxes.append( br );
    tmp.addRect( br );
  }
  bBox = tmp.boundingRect();
  exactShape=tmp;
  update();
}

hilightAnnotation::hilightAnnotation( hilightTool *tool, PoDoFo::PdfAnnotation *hilightAnnot, pdfCoords *transform): 
	abstractAnnotation(tool, hilightAnnot, transform), bBox(0,0,0,0) {
	  movable=false;
	  if ( isA( hilightAnnot ) ) {
	    PoDoFo::PdfArray quadPoints = hilightAnnot->GetQuadPoints();
	    QRectF tmp;
	    QList<QRectF> boxes = pdfUtil::quadPointsToQBoxes( quadPoints, transform );
	    foreach( QRectF box, boxes ) { 
	      tmp =  QRectF( box.x()-pos().x(), box.y()-pos().y(), box.width(), box.height() );
	      hBoxes.append( tmp );
	      exactShape.addRect( tmp );
	    }
	    bBox = exactShape.boundingRect();
	  }
	  setZValue( 9 );
};

bool hilightAnnotation::isA( PoDoFo::PdfAnnotation *annotation ) { 
  if ( ! annotation ) return false;
  return ( annotation->GetType() == PoDoFo::ePdfAnnotation_Highlight );
}
void hilightAnnotation::saveToPdfPage( PoDoFo::PdfDocument *document, PoDoFo::PdfPage *pg, pdfCoords *coords ) { 
  qDebug() << "Saving HILIGHT annotation for "<<getAuthor()<<" : " << pos();
  QList<QRectF> pageBoxes;
  foreach( QRectF box, hBoxes ) { 
    pageBoxes.append( mapToParent(box).boundingRect() );
  }
  PoDoFo::PdfRect *brect = coords->sceneToPdf( mapToParent(bBox).boundingRect() );
  PoDoFo::PdfArray quadPoints =  pdfUtil::qBoxesToQuadPoints( pageBoxes, coords );
  PoDoFo::PdfAnnotation *annot = pg->CreateAnnotation( PoDoFo::ePdfAnnotation_Highlight, *brect );
  annot->SetQuadPoints( quadPoints );
  saveInfo2PDF( annot );
  annot->SetColor( 0, 0, 1, 0 ); // Set the annotation to be yellow
  delete brect;
}



#include "hilightTool.moc"
