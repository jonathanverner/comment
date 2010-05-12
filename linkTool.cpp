/**  This file is part of project comment
 *
 *  File: linkTool.cpp
 *  Created: 02.05.2010
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

#include "linkTool.h"

#include "toolBox.h"
#include "linkTool.h"
#include "pdfScene.h"
#include "pdfUtil.h"
#include "linkLayer.h"
#include "pageView.h"

#include <QtGui/QTextEdit>
#include <QtGui/QStackedWidget>

#include <QtCore/QDebug>

#include <podofo/podofo.h>



QIcon linkTool::icon;

bool linkTool::acceptEventsFor( QGraphicsItem *item ) {
 if ( dynamic_cast<linkAnnotation*>(item) ) {
    return true;
  }
  return false;
}

linkTool::linkTool( pdfScene *Scene, toolBox *ToolBar, QStackedWidget *EditArea ):
        abstractTool( Scene, ToolBar, EditArea )
{
  setToolName( "Link Tool" );
  icon = QIcon::fromTheme("link");
  toolBar->addTool( QIcon(icon), this );
}

abstractAnnotation *linkTool::processAnnotation( PoDoFo::PdfAnnotation *annotation, pdfCoords *transform ) {
  if ( ! linkAnnotation::isA( annotation ) ) return NULL;
  try {
    return new linkAnnotation( this, annotation, transform );
  } catch (...) {
    qDebug() << "Error adding annotation";
    return NULL;
  }
}

void linkTool::newActionEvent( const QPointF *ScenePos ) {
/*  qDebug() << "Creating new Link Annotation at " << *ScenePos;
  linkAnnotation *annot = new linkAnnotation( this );
  annot->setZValue( 10 );
  scene->placeAnnotation( annot, ScenePos );
  editItem( annot );*/
}



linkAnnotation::linkAnnotation( linkTool* tool, PoDoFo::PdfAnnotation* Link, pdfCoords* transform ):
        abstractAnnotation( tool, Link, transform )
{
  movable = false;
  PoDoFo::PdfDestination *dest = pdfUtil::getDestination( Link );
  if ( ! dest ) throw;
  QString tgtName="";
  if ( dest->GetObject()->IsString() ) {
    tgtName = pdfUtil::pdfStringToQ(dest->GetObject()->GetString());
  } else if ( dest->GetObject()->GetDictionary().HasKey(PoDoFo::PdfName("comment_target_name")) ) {
    tgtName = pdfUtil::pdfStringToQ(dest->GetObject()->GetDictionary().GetKey(PoDoFo::PdfName("comment_target_name"))->GetString());
  }
  tgt = tool->scene->getLinkTargets()->addTarget( tgtName, dest );
  PoDoFo::PdfRect pdfRect = Link->GetRect();
  QRectF pgRect = transform->pdfRectToScene( &pdfRect );
  activeArea=QRectF(QPointF(0,0),pgRect.size());
  setPos(pgRect.topLeft());
  setZValue( 10 );
  qDebug() << "Created new link from " << pos() << " ["<<activeArea<<"] to " << tgt->pos() << " ["<<tgt->boundingRect()<<"]";
}


bool linkAnnotation::isA( PoDoFo::PdfAnnotation *annotation ) {
  if ( ! annotation ) return false;
  return ( annotation->GetType() == PoDoFo::ePdfAnnotation_Link );
}


void linkAnnotation::saveToPdfPage( PoDoFo::PdfDocument *document, PoDoFo::PdfPage *pg, pdfCoords *coords ) {
  qDebug() << "Saving Link annotation for "<<getAuthor()<<" : " << pos();
  PoDoFo::PdfRect *brect = coords->sceneToPdf( mapToParent(activeArea).boundingRect() );
  PoDoFo::PdfAnnotation *annot = pg->CreateAnnotation( PoDoFo::ePdfAnnotation_Link, *brect );
  PoDoFo::PdfDestination *dest = tgt->getPdfDest(pg);
  annot->SetDestination( *dest );
  saveInfo2PDF( annot );
  delete brect;
  delete dest;
  
}

void linkAnnotation::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
  painter->fillRect( activeArea, QColor( 155, 155, 0, 100 ) );
}

bool linkTool::handleEvent(viewEvent* ev) {
  linkAnnotation *annot;
  if ( ev->type() == viewEvent::VE_MOUSE_PRESS && ( ev->btnCaused() == Qt::LeftButton ) ) {
    if ( annot = dynamic_cast<linkAnnotation*>(ev->item()) ) { 
      //editAnnotationExtent( annot );
    } else {
      QPointF pos = ev->scenePos();
      newActionEvent( &pos );
    }
    return true;
  } else if ( ev->type() == viewEvent::VE_MOUSE_RELEASE && ( ev->btnCaused() == Qt::LeftButton ) ) { 
    if ( ev->isClick() && (annot = dynamic_cast<linkAnnotation*>(ev->item())) ) {
      qDebug() << "Going to " << annot->tgt->getName() << " at " << annot->tgt->scenePos();
      emit gotoPos( annot->tgt->scenePos() );
      return true;
    }
    if ( ! (annot=dynamic_cast<linkAnnotation*>(currentEditItem)) ) return false;
    //updateCurrentAnnotation( ev->scenePos() );
    //editAnnotationText();
    return true;
  } else if ( ev->type() == viewEvent::VE_MOUSE_MOVE && (annot=dynamic_cast<linkAnnotation*>(currentEditItem)) ) {
    /*if ( editingHilight ) {
      updateCurrentAnnotation( ev->scenePos() );
      if ( ! (ev->btnState() & Qt::LeftButton) ) { // Missed a mouse release, end editing annotation
        qDebug() << "WARNING MISSED MOUSE RELEASE EVENT!!!";
        editAnnotationText();
      }
    } else finishEditing();*/
    return true;  
  } else return abstractTool::handleEvent( ev );
  return false;
}


#include "linkTool.moc"
