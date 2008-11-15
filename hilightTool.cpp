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
#include <QtCore/QDebug>
#include <QtGui/QIcon>
#include <QtGui/QStackedWidget>


#include "hilightTool.h"
#include "pageView.h"
#include "textLayer.h"
#include "toolBox.h"
#include "pdfScene.h"
#include "pdfPageItem.h"
#include "pdfUtil.h"

QPixmap *hilightTool::icon = NULL;

hilightTool::hilightTool( pdfScene *Scene, toolBox *ToolBar, QStackedWidget *EditArea):
	abstractTool( Scene, ToolBar, EditArea )
{
  setToolName( "Hilight Tool" );
  if ( ! icon ) icon = new QPixmap( "hilight.png" );
  toolBar->addTool( QIcon(*icon), this );
}


void hilightTool::newActionEvent( const QPointF *ScenePos ) {
  hilightAnnotation *hi = new hilightAnnotation(this);
  scene->placeAnnotation( hi, ScenePos );
  hi->setZValue( 9 );
  qDebug() << "Editing item";
  editItem( hi );
}

void hilightTool::updateCurrentAnnotation( QPointF ScenePos ) { 
  hilightAnnotation *annot = dynamic_cast<hilightAnnotation*>(currentEditItem); 
  Q_ASSERT( annot );
  QPointF from = annot->pos();
  QPointF to = annot->parentItem()->mapFromScene( ScenePos );
  textLayer *txt;
  foreach( QGraphicsItem *item, scene->items( ScenePos ) ) {
	  if ( dynamic_cast<pdfPageItem*>(item) ) { 
	    foreach( QGraphicsItem *i, item->children() ) { 
	      if ( txt = dynamic_cast<textLayer*>(i) ) {
		annot->updateSelection( txt->select( from, to ) );
		return;
	      }

	    }
	  }
  }
  return;
}

bool hilightTool::acceptEventsFor( QGraphicsItem *item ) {  
  return dynamic_cast<hilightAnnotation*>( item );
}

bool hilightTool::handleEvent( viewEvent *ev ) { 
  hilightAnnotation *annot;
  if ( ev->type() == viewEvent::VE_MOUSE_PRESS && ( ev->btnCaused() == Qt::LeftButton ) ) {
    if ( annot = dynamic_cast<hilightAnnotation*>(ev->item()) ) { 
      editItem( annot );
    } else {
      QPointF pos = ev->scenePos();
      newActionEvent( &pos );
    }
    return true;
  } else if ( ev->type() == viewEvent::VE_MOUSE_RELEASE && ( ev->btnCaused() == Qt::LeftButton ) ) { 
    if ( ! (annot=dynamic_cast<hilightAnnotation*>(currentEditItem)) ) return false;
    if ( ev->isClick() ) {
      return true;
    }
    updateCurrentAnnotation( ev->scenePos() );
    currentEditItem = NULL;
    editArea->hide();
    return true;
  } else if ( ev->type() == viewEvent::VE_MOUSE_MOVE && (annot=dynamic_cast<hilightAnnotation*>(currentEditItem)) ) { 
    updateCurrentAnnotation( ev->scenePos() );
     if ( ! (ev->btnState() & Qt::LeftButton) ) { // Missed a mouse release, end editing annotation
      qDebug() << "WARNING MISSED MOUSE RELEASE EVENT!!!";
      currentEditItem=NULL;
      editArea->hide();
      return true;
    };
    return true;
  }
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

void hilightAnnotation::updateSelection( QList<QRectF> newSelection ) { 
  QPainterPath tmp;
  QRectF br;
  update();
  prepareGeometryChange();
  hBoxes.clear();
  foreach( QRectF box, newSelection ) { 
    br = mapFromParent( box ).boundingRect();
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
  PoDoFo::PdfAnnotation *annot = pg->CreateAnnotation( PoDoFo::ePdfAnnotation_Highlight, *brect, &quadPoints );
  saveInfo2PDF( annot );
  annot->SetColor( 0, 0, 1, 0 ); // Set the annotation to be yellow
  delete brect;
}



#include "hilightTool.moc"
