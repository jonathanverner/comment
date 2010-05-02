/**  This file is part of project comment
 *
 *  File: pageBeginItem.cpp
 *  Created: 2008-10-17
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




pageBeginItem::pageBeginItem( int pgNum, qreal w, QGraphicsItem *parent ) : QGraphicsItem(parent), pageNum( pgNum ), wd(w) { 
  sg = new pageBeginSignalEmitter();
}

pageBeginItem::~pageBeginItem() {
}



void pageBeginItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget ) {
  sg->emitSignal( pageNum );
}




#include "pageBeginItem.moc" 
