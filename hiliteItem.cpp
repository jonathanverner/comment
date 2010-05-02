/**  This file is part of project comment
 *
 *  File: hiliteItem.cpp
 *  Created: 2009-02-13
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



#include "hiliteItem.h"

#include <QtCore/QDebug>

#include <poppler-qt4.h>

using namespace Poppler;

hiliteItem::hiliteItem( QPointF topLeftPage, QList<TextBox *> bboxes ):
	col( 0, 255, 217, 100 ), bBox( 0, 0, 0, 0 ), active(false)
{ 
  setPos( topLeftPage );
  updateBBoxes( bboxes );
}

hiliteItem::hiliteItem():
	col( 0, 0, 0, 100), bBox( 0, 0, 0, 0 ), active(false)
{
  setPos( QPointF(0,0) );
  hide();
}

void hiliteItem::setColor( QColor cl ) { 
  col = cl;
  update();
}

void hiliteItem::setActive() {
  active=true;
  col = QColor( 0, 255, 217, 100 );
  update();
}

void hiliteItem::setInactive() {
  active=false;
  col = QColor( 105, 255, 210, 100);
  update();
}

QRectF hiliteItem::boundingRect() const { 
  return bBox;
}

void hiliteItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget ) {
  QPointF myPos = pos();
  foreach( QRectF box, hBoxes ) { 
    painter->fillRect( box, col );
    if (active) painter->drawRect( box );
  }
}

QPainterPath hiliteItem::shape() const { 
  return exactShape;
}

void hiliteItem::clear() { 
  hide();
  update();
  prepareGeometryChange();
  setPos(QPointF(0,0));
  hBoxes.clear();
  bBox = QRectF(0,0,0,0);
  exactShape = QPainterPath();
  update();
}

void hiliteItem::updateBBoxes( QList<TextBox*> bboxes ) { 
  QPainterPath tmp;
  QRectF br;
  update();
  prepareGeometryChange();
  hBoxes.clear();
  foreach( TextBox *box, bboxes ) { 
    br = box->boundingBox();
    hBoxes.append( br );
    tmp.addRect( br );
  }
  bBox = tmp.boundingRect();
  exactShape=tmp;
  update();
}
