/**  This file is part of project comment
 *
 *  File: wordItem.cpp
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


#include "wordItem.h"

#include <QtGui/QPainter>
#include <poppler-qt4.h> 

using namespace Poppler;

wordItem::wordItem( TextBox *box ): bx(box), hilited(false) {
  setZValue(5);
  w=bx->boundingBox().width();
  h=bx->boundingBox().height();
  setPos( bx->boundingBox().x(), bx->boundingBox().y() );
};

void wordItem::hilight( bool on ) {
  hilited = on;
}

QString wordItem::word() {
  return bx->text();
}

QRectF wordItem::boundingRect() const {
  return QRectF( 0,0,w,h );
}
void wordItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget ) {
  if (hilited) { 
    painter->fillRect(QRectF(0,0,w,h)/* bx->boundingBox()*/, QColor(255,255,0,128) );
  } else {
//    painter->drawRect(QRectF(0,0,w,h)/* bx->boundingBox()*/);
  }
}

