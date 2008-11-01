/***************************************************************
 * testView.cpp
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
#include "pageBeginItem.h"
#include "testView.h"
#include "abstractTool.h"



void testView::mouseMoveEvent( QMouseEvent *e ) { 
  if ( e->pos().y() < 30 || e->pos().y() > (height()-30) || 
       e->pos().x() < 30 || e->pos().x() > (width()-30) ) {
    emit mouseNearBorder( e->globalPos() );
  }
  QGraphicsView::mouseMoveEvent( e );
  if ( moving ) moved = true;
}

void testView::mousePressEvent( QMouseEvent *e ) { 
  moving = true;
  QGraphicsView::mousePressEvent( e );
}


void testView::mouseReleaseEvent( QMouseEvent *e ) { 
  moving = false;
  foreach( QGraphicsItem *i, items( e->pos() ) ) { 
    if ( dynamic_cast<abstractAnnotation*>( i ) ) {
        QGraphicsView::mouseReleaseEvent( e );
	moved = false;
	return;
    }
  }
  QGraphicsView::mouseReleaseEvent( e );
  qDebug() << "MouseReleaseEvent isAccepted:" << e->isAccepted();
  if ( e->button() == Qt::LeftButton && ! moved ) { 
    emit newAnnotationAction( mapToScene( e->pos() ) );
  }
  moved=false;
}
    


void testView::zoomIN() {
  scale ( 1.5, 1.5 );
}

void testView::zoomOUT() {
  scale( 0.7, 0.7 ); 
}

int testView::getCurrentPage() { 
  pageBeginItem *pg;
  foreach( QGraphicsItem *i, items() ) { 
    if ( pg = dynamic_cast<pageBeginItem *>(i) ) { 
      currentPage = pg->getPageNum();
      return currentPage;
    }
  }
  return currentPage;
}

void testView::gotoPage( int num ) { 
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

void testView::nextPage() { 
  gotoPage( currentPage + 1 );
}

void testView::prevPage() { 
  gotoPage( currentPage - 1 );
}

void testView::up() {
}

void testView::down() {
}

void testView::left() {
}

void testView::right() {
}

#include "testView.moc"
