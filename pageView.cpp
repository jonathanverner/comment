/**  This file is part of project comment
 *
 *  File: pageView.cpp
 *  Created: 2008-10-12
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


#include "pageBeginItem.h"
#include "pageView.h"
#include "abstractTool.h"
#include "myToolTip.h"
#include "hiliteItem.h"
#include "pdfScene.h"

#include <QtCore/QDebug>
#include <QtGui/QApplication>
#include <QtGui/QClipboard>
#include <QtGui/QMouseEvent>
#include <QtGui/QKeyEvent>
#include <QtGui/QGraphicsItem>
#include <QtGui/QScrollBar>
#include <QtGui/QMenu>



bool viewEvent::isClick() { 
  return distance < 10;
};

QAction *pageView::newAction( QString shortCut, QObject *target, const char* slot ) { 
  return newAction( QKeySequence(shortCut), target, slot );
}

QAction *pageView::newAction( QKeySequence shortCut, QObject *target, const char* slot ) {
  QAction *ret = new QAction( this );
  ret->setShortcut( shortCut );
  ret->setShortcutContext( Qt::WidgetShortcut );
  addAction( ret );
  connect( ret, SIGNAL( triggered() ), target, slot );
  return ret;
}

void pageView::disableActions( bool b) {
  qDebug() << "pageView: Disabling actions " << b;
  QAction *act;
  foreach( act, actions() ) {
    act->setDisabled(b);
  }
}

pageView::pageView( QGraphicsScene *scene, QWidget *parent ) :
	QGraphicsView( scene, parent ), zoom(1), currentPage(1), currentTool(NULL),
	movingItem(NULL), toolTipItem(NULL) { 
	  setDragMode( QGraphicsView::ScrollHandDrag );
	  hi = new hiliteItem();
	  hi->setColor( QColor(0,0,0,100) );
	  hi->setZValue( 40 );
	  scene->addItem(hi);
	}


viewEvent pageView::eventToVE( QMouseEvent *e, viewEvent::eventType tp ) { 
 viewEvent ret;
 abstractAnnotation *annot;
 ret.distance=0;
 ret.viewPort = viewport();
 ret.original_event = e;
 if ( tp == viewEvent::VE_MOUSE_PRESS ) { 
   mousePressStartPos = e->pos();
 } else if ( tp == viewEvent::VE_MOUSE_MOVE && e->buttons() ) {
   QPoint delta = e->pos() - mousePressStartPos;
   ret.distance = abs(delta.x())+abs(delta.y());
   qDebug() << "Distance:" << ret.distance;
 } else if ( tp == viewEvent::VE_MOUSE_RELEASE ) {
   QPoint delta = e->pos() - mousePressStartPos;
   ret.distance = abs(delta.x())+abs(delta.y());
   mousePressStartPos.setX(-1);
   mousePressStartPos.setY(-1);
   qDebug() << "Total: Distance:" << ret.distance;
 }
 
 ret.mousePressPosition=mapToScene(mousePressStartPos);
 ret.myType = tp;
 ret.SC = scene();
 ret.IT=NULL;
 QList<QGraphicsItem*> itms = items(e->pos());
 for( QList<QGraphicsItem*>::iterator it = itms.begin(); it != itms.end(); ++it ) {
   ret.topMostAll = dynamic_cast<abstractAnnotation*>(*it);
   if ( ret.topMostAll ) break;
 }
 ret.lastSP=mapToScene( lastMouseEvPos );
 ret.SP=mapToScene( e->pos() );
 ret.bt_caused = e->button();
 ret.bt_state = e->buttons();
 ret.evPos=e->pos();
 ret.lastEvPos=lastMouseEvPos;
 lastMouseEvPos=e->pos();
 if ( currentTool ) { 
   foreach( QGraphicsItem *i, items( e->pos() ) ) {
     if ( currentTool->acceptEventsFor( i ) ) {
       ret.IT = i;
       break;
     }
   }
 }
 return ret;
};

void pageView::setCurrentTool( abstractTool *tool ) { 
  currentTool = tool;
}

/*void pageView::keyPressEvent( QKeyEvent *e ) { 
  return;
};*/

void pageView::mouseMoveEvent( QMouseEvent *e ) { 
  viewEvent viewEv = eventToVE( e, viewEvent::VE_MOUSE_MOVE );
  if ( e->pos().y() < 30 || e->pos().y() > (height()-30) || 
       e->pos().x() < 30 || e->pos().x() > (width()-30) ) {
    emit mouseNearBorder( e->globalPos() );
  }
  if ( movingItem ) { // moving an item
    movingItem->setPos( mapToScene( e->pos() ) - movingItem->scenePos() + movingItem->pos()+moveDelta );
  } else if ( currentTool && currentTool->handleEvent( &viewEv ) ) { 
    return;
  } else if ( viewEv.bt_state & Qt::LeftButton ) { // panning the view
    QScrollBar *hBar = horizontalScrollBar();
    QScrollBar *vBar = verticalScrollBar();
    QPoint delta = viewEv.eventDelta();
    hBar->setValue(hBar->value() - (isRightToLeft() ? delta.x() : -delta.x()));
    vBar->setValue(vBar->value() + delta.y());
/*    pdfScene *sc = dynamic_cast<pdfScene*>(scene());
    qDebug() << "Currently on Pos:" << hBar->value()<< vBar->value();*/
  } else if ( viewEv.bt_state & Qt::RightButton ) { 
    hi->updateBBoxes( static_cast<pdfScene*>(scene())->selectText( viewEv.mousePressPosition, viewEv.scenePos() ) );
    selectedText = static_cast<pdfScene*>(scene())->selectedText( viewEv.mousePressPos(), viewEv.scenePos( ) );
  } else { // show/hide tooltips
    abstractAnnotation *annot;
    bool hide = true;
    foreach( QGraphicsItem *i, items( viewEv.evPos ) ) { 
      if ( annot = dynamic_cast<abstractAnnotation*>( i ) ) {
	if ( annot->hasToolTip() ) {
	  if ( toolTipItem &&  toolTipItem != annot ) {
	    qDebug() << "Hiding old tooltip to show new";
	    toolTipItem->hideToolTip();
	  } else if ( toolTipItem == annot ) { 
	    qDebug() << "Tooltip Shown, no need to show more";
	    hide = false;
	    break;
	  }
	  toolTipItem = annot;
	  toolTipItem->showToolTip( e->globalPos()+QPoint(10,10) );
	  hide=false;
	  break;
	}
      }
    }
    if ( toolTipItem && hide ) {
      myToolTip::hide();
      if ( currentTool ) currentTool->hideEditor();
      toolTipItem = NULL;
    }
  }
}

void pageView::mousePressEvent( QMouseEvent *e ) { 
  viewEvent viewEv = eventToVE( e, viewEvent::VE_MOUSE_PRESS );
  viewport()->setCursor(Qt::ClosedHandCursor);
  if ( viewEv.bt_state & Qt::LeftButton ) { // check whether we will be moving an item
    abstractAnnotation *annot;
    foreach( QGraphicsItem *i, items( viewEv.evPos ) ) {
      if ( annot = dynamic_cast<abstractAnnotation*>( i ) ) { 
	if ( annot->isMovable() ) {
	  qDebug() << " Moving annotation ";
	  movingItem = i;
	  moveDelta = annot->scenePos() - viewEv.SP;
	  return;
	}
      }
    }
    if ( currentTool ) currentTool->handleEvent( &viewEv );
  } else if ( (viewEv.bt_caused & Qt::RightButton) && viewEv.topMostAll ) { // popup-menu
    viewEv.topMostAll->contextMenu()->popup( e->globalPos() );
  } else if ( currentTool ) 
    currentTool->handleEvent( &viewEv );
  else if ( viewEv.bt_caused & Qt::RightButton ) {
    hi->clear();
    hi->setPos( static_cast<pdfScene*>(scene())->topLeftPage(static_cast<pdfScene*>(scene())->posToPage( viewEv.scenePos() ) ) );
    hi->show();
    selectedText = "";
  }
}

void pageView::mouseReleaseEvent( QMouseEvent *e ) { 
  viewport()->setCursor(Qt::OpenHandCursor);
  viewEvent viewEv = pageView::eventToVE( e, viewEvent::VE_MOUSE_RELEASE );
  movingItem=NULL;
  if ( currentTool ) {
    if ( currentTool->handleEvent( &viewEv ) ) return;
  };
  if ( viewEv.isClick() && ( viewEv.bt_caused == Qt::LeftButton ) ) {
    abstractAnnotation *annot;
    foreach( QGraphicsItem *i, items( viewEv.evPos ) ) {
      if ( annot = dynamic_cast<abstractAnnotation*>( i ) ) { 
	if ( annot->editSelf() ) return;
      }
    }
  }
  if ( viewEv.bt_caused == Qt::RightButton ) {
    qDebug() << "Selected: " << selectedText;
    QApplication::clipboard()->setText( selectedText, QClipboard::Selection );
  }
}

    


void pageView::zoomIN() {
  scale ( 1.5, 1.5 );
}

void pageView::zoomOUT() {
  scale( 0.7, 0.7 ); 
}

int pageView::getLastPage() {
  int lastPageNum = 0;
  pageBeginItem *pg;
  foreach( QGraphicsItem *i, items() ) { 
    if ( pg = dynamic_cast<pageBeginItem *>(i) )
      if ( pg->getPageNum() > lastPageNum ) lastPageNum = pg->getPageNum();
  }
  return lastPageNum;
}
		      

void pageView::gotoPage( int num ) { 
  pageBeginItem *pg;
  foreach( QGraphicsItem *i, items() ) { 
    if ( pg = dynamic_cast<pageBeginItem *>(i) ) { 
      if  ( pg->getPageNum() == num ) { 
	currentPage = num;
	centerOn( i );
	emit onPage( num );
	return;
      }
    }
  }
  return;
}

void pageView::gotoPoint(const QPointF& point) {
  centerOn( point );
  //FIXME: Need to emit onPage
}


void pageView::firstPage() { 
  gotoPage( 1 );
}

void pageView::lastPage() { 
  gotoPage( getLastPage() );
}

void pageView::nextPage() { 
  gotoPage( currentPage + 1 );
}

void pageView::prevPage() { 
  gotoPage( currentPage - 1 );
}

void pageView::up() {
  QScrollBar *hBar = horizontalScrollBar();
  QScrollBar *vBar = verticalScrollBar();
  int delta = 10;
  vBar->setValue(vBar->value() - delta);
}

void pageView::down() {
  QScrollBar *vBar = verticalScrollBar();
  int delta = 10;
  vBar->setValue(vBar->value() + delta);
}

void pageView::left() {
  QScrollBar *hBar = horizontalScrollBar();
  int delta = 10;
  hBar->setValue(hBar->value() - delta);

}

void pageView::right() {
  QScrollBar *hBar = horizontalScrollBar();
  int delta = 10;
  hBar->setValue(hBar->value() + delta);
}

#include "pageView.moc"
