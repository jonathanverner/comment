/***************************************************************
 * pageView.cpp
 * @Author:      Jonathan Verner (jonathan.verner@matfyz.cz)
 * @License:     GPL v2.0 or later
 * @Created:     2008-10-12.
 * @Last Change: 2008-10-12.
 * @Revision:    0.0
 * Description:
 * Usage:
 * TODO:
 *CHANGES:
 ***************************************************************/
#include <QtCore/QDebug>
#include <QtGui/QMouseEvent>
#include <QtGui/QKeyEvent>
#include <QtGui/QGraphicsItem>
#include <QtGui/QScrollBar>
#include <QtGui/QMenu>
#include "pageBeginItem.h"
#include "pageView.h"
#include "abstractTool.h"
#include "myToolTip.h"
//#include "pdfScene.h"


bool viewEvent::isClick() { 
  return distance < 10;
};

pageView::pageView( QGraphicsScene *scene, QWidget *parent ) :
	QGraphicsView( scene, parent ), zoom(1), currentPage(1), currentTool(NULL),
	movingItem(NULL), toolTipItem(NULL) { 
	  setDragMode( QGraphicsView::ScrollHandDrag );
	}


viewEvent pageView::eventToVE( QMouseEvent *e, viewEvent::eventType tp ) { 
 viewEvent ret;
 abstractAnnotation *annot;
 ret.distance=0;
 ret.viewPort = viewport();
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
  
 ret.myType = tp;
 ret.SC = scene();
 ret.IT=NULL;
 ret.topMostAll = dynamic_cast<abstractAnnotation*>(itemAt(e->pos()));
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

void pageView::keyPressEvent( QKeyEvent *e ) { 
  return;
};

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
  } else if ( currentTool ) currentTool->handleEvent( &viewEv );
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
}

    


void pageView::zoomIN() {
  scale ( 1.5, 1.5 );
}

void pageView::zoomOUT() {
  scale( 0.7, 0.7 ); 
}

int pageView::getCurrentPage() { 
  pageBeginItem *pg;
  foreach( QGraphicsItem *i, items() ) { 
    if ( pg = dynamic_cast<pageBeginItem *>(i) ) { 
      currentPage = pg->getPageNum();
      return currentPage;
    }
  }
  return currentPage;
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

void pageView::nextPage() { 
  gotoPage( currentPage + 1 );
}

void pageView::prevPage() { 
  gotoPage( currentPage - 1 );
}

void pageView::up() {
}

void pageView::down() {
}

void pageView::left() {
}

void pageView::right() {
}

#include "pageView.moc"
