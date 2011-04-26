/**  This file is part of project comment
 *
 *  File: hilightTool.cpp
 *  Created: 2008-11-07
 *  Author: Jonathan Verner <jonathan.verner@matfyz.cz>
 *  License: GPL v2 or later
 *
 *  Copyright (C) 2010 Jonathan Verner <jonathan.verner@matfyz.cz>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 */



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
#include <QtGui/QComboBox>

#include <poppler-qt4.h>
#include <podofo/podofo.h>

using namespace Poppler;


enum hilightAnnotation::hilightType PoDoFoToPdfType(const int podofoTyp)  {
  switch( podofoTyp ) { 
    case PoDoFo::ePdfAnnotation_Squiggly:
      return hilightAnnotation::squiggly;
    case PoDoFo::ePdfAnnotation_Underline:
      return hilightAnnotation::underline;
    case PoDoFo::ePdfAnnotation_StrikeOut:
      return hilightAnnotation::strikeout;
    case PoDoFo::ePdfAnnotation_Highlight:
      return hilightAnnotation::hilight;
    default:
      return hilightAnnotation::undefined;
  }
}

enum PoDoFo::EPdfAnnotation PdfToPoDoFoType(hilightAnnotation::hilightType t) {
  switch( t ) { 
    case hilightAnnotation::hilight:
      return PoDoFo::ePdfAnnotation_Highlight;
    case hilightAnnotation::squiggly:
      return PoDoFo::ePdfAnnotation_Squiggly;
    case hilightAnnotation::underline:
      return PoDoFo::ePdfAnnotation_Underline;
    case hilightAnnotation::strikeout:
      return PoDoFo::ePdfAnnotation_StrikeOut;
  }
}


QIcon hilightTool::icon;



hilightTool::hilightTool( pdfScene *Scene, toolBox *ToolBar, QStackedWidget *EditArea):
	abstractTool( Scene, ToolBar, EditArea ), editingHilight(false)
{
  icon = QIcon::fromTheme("format-text-underline");
  setToolName( "Hilight Tool" );
  typSelect = new QComboBox();
  typSelect->addItem("Hilight");
  typSelect->addItem("Underline");
  typSelect->addItem("Squiggly underline");
  typSelect->addItem("Strikeout");
  connect( typSelect, SIGNAL( currentIndexChanged(const QString &)), this, SLOT( updateTyp( const QString & )) );
  toolBar->addTool( icon, this );
  propertyEdit->addWidgetWithLabel( "Type:", typSelect );
}


void hilightTool::newActionEvent( const QPointF *ScenePos ) {
  qDebug() << "New action event...";
  hilightAnnotation *hi = new hilightAnnotation(this);
  scene->placeAnnotation( hi, ScenePos );
  hi->setZValue( 9 );
  qDebug() << "Editing item";
  editAnnotationExtent( hi );
}

void hilightTool::updateTyp(const QString& typ) {
  qDebug() << "hilightTool::updateTyp("<<typ<<");";
  hilightAnnotation *annot = dynamic_cast<hilightAnnotation*>(currentEditItem);
  if ( ! annot ) return;
  if ( typ == "Hilight" ) annot->setTyp( hilightAnnotation::hilight );
  if ( typ == "Underline" ) annot->setTyp( hilightAnnotation::underline );
  if ( typ == "Squiggly underline" ) annot->setTyp( hilightAnnotation::squiggly );
  if ( typ == "Strikeout" ) annot->setTyp( hilightAnnotation::strikeout );
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

void hilightTool::prepareEditItem(abstractAnnotation* item) {
  abstractTool::prepareEditItem(item);
  hilightAnnotation *annot = dynamic_cast<hilightAnnotation*>(currentEditItem); 
  if ( annot ) typSelect->setCurrentIndex(annot->typ);
  editingHilight=false;
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
      if ( editingHilight ) prepareEditItem(currentEditItem);
      else finishEditing();
      return true;
    }
    updateCurrentAnnotation( ev->scenePos() );
    prepareEditItem(currentEditItem);
    return true;
  } else if ( ev->type() == viewEvent::VE_MOUSE_MOVE && (annot=dynamic_cast<hilightAnnotation*>(currentEditItem)) ) {
    if ( editingHilight ) {
      updateCurrentAnnotation( ev->scenePos() );
      if ( ! (ev->btnState() & Qt::LeftButton) ) { // Missed a mouse release, end editing annotation
        qDebug() << "WARNING MISSED MOUSE RELEASE EVENT!!!";
        editItem(currentEditItem);
      }
    } else finishEditing();
    return true;  
  } else return abstractTool::handleEvent( ev );
  return false;
}

abstractAnnotation *hilightTool::processAnnotation( PoDoFo::PdfDocument* doc, PoDoFo::PdfAnnotation* annotation, pdfCoords* transform ) {
  if ( ! hilightAnnotation::isA( annotation ) ) return NULL;
  return new hilightAnnotation( this, hilightAnnotation::hilight, annotation, transform );
}


void hilightAnnotation::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget ) {
  QPointF myPos = pos();
  QColor my_col = getColor();
  if ( ! my_col.isValid() ) my_col = QColor( 255, 255, 0 );
  painter->setPen( my_col );
  my_col.setAlpha(156);
  foreach( QRectF box, hBoxes ) {
    switch( typ ) { 
      case hilight:
	painter->fillRect( box, my_col );
	break;
      case squiggly:
      case underline:
	painter->drawLine( box.bottomLeft(), box.bottomRight() );
	break;
      case strikeout:
	painter->drawLine( box.topLeft()+QPoint(0,box.height()/2), box.topRight()+QPoint(0,box.height()/2) );
	break;
    }
  }
}



QPainterPath hilightAnnotation::shape() const { 
  return exactShape;
}

void hilightAnnotation::setTyp(hilightAnnotation::hilightType tp) {
  update();
  prepareGeometryChange();
  typ=tp;
  update();
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

hilightAnnotation::hilightAnnotation( hilightTool *tool, enum hilightType tp, PoDoFo::PdfAnnotation *hilightAnnot, pdfCoords *transform): 
	abstractAnnotation(tool, hilightAnnot, transform), bBox(0,0,0,0), typ(tp) {
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
	    typ = PoDoFoToPdfType( hilightAnnot->GetType() );
	  }
	  setZValue( 9 );
};

bool hilightAnnotation::isA( PoDoFo::PdfAnnotation *annotation ) { 
  if ( ! annotation ) return false;
  return ( PoDoFoToPdfType( annotation->GetType() ) != undefined );
}



void hilightAnnotation::saveToPdfPage( PoDoFo::PdfDocument *document, PoDoFo::PdfPage *pg, pdfCoords *coords ) { 
  qDebug() << "Saving HILIGHT annotation for "<<getAuthor()<<" : " << pos();
  QList<QRectF> pageBoxes;
  foreach( QRectF box, hBoxes ) { 
    pageBoxes.append( mapToParent(box).boundingRect() );
  }
  PoDoFo::PdfRect *brect = coords->sceneToPdf( mapToParent(bBox).boundingRect() );
  PoDoFo::PdfArray quadPoints =  pdfUtil::qBoxesToQuadPoints( pageBoxes, coords );
  PoDoFo::PdfAnnotation *annot = pg->CreateAnnotation( PdfToPoDoFoType( typ ), *brect );
  annot->SetQuadPoints( quadPoints );
  saveInfo2PDF( annot );
  //annot->SetColor( 0, 0, 1, 0 ); // Set the annotation to be yellow
  delete brect;
}



#include "hilightTool.moc"
