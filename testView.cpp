/**  This file is part of project comment
 *
 *  File: testView.cpp
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
