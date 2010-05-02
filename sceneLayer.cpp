/**  This file is part of project comment
 *
 *  File: sceneLayer.cpp
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


 
#include "sceneLayer.h"

#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsItem>

#include <QtCore/QDebug>

sceneLayer::sceneLayer( QGraphicsScene *SC):
	scene(SC), cItem(0), zVal(0)
{
}

void sceneLayer::setZValue( int zV ) { 
  zVal = zV;
  foreach( QGraphicsItem *it, items ) { 
    it->setZValue( zVal );
  }
}

void sceneLayer::addItem( QGraphicsItem *item, bool addToScene) {
  items.append(item);
  reference.append( ! addToScene );
  if ( addToScene ) scene->addItem( item );
  item->setZValue( zVal );
}

QGraphicsItem *sceneLayer::currentItem() {
  Q_ASSERT( cItem >= 0 );
  if ( cItem < items.size() ) return items[cItem];
  else return NULL;
}

void sceneLayer::advanceCurrentItem( int i ) {
  if ( items.size() <= 0 ) return;
  cItem+=i;
  while ( cItem >= items.size() ) cItem-=items.size();
  while ( cItem < 0 ) cItem +=items.size();
}

void sceneLayer::hide() {
  foreach( QGraphicsItem *it, items ) { 
    it->hide();
  }
}

void sceneLayer::show() {
  foreach( QGraphicsItem *it, items ) { 
    it->show();
  }
}

void sceneLayer::clear() {
  for(int i=0; i < items.size(); i++ ) { 
    if ( ! reference[i] ) {
      scene->removeItem( items[i] );
      delete items[i];
    }
  }
  items.clear();
  reference.clear();
  cItem=0;
}

#include "sceneLayer.moc"
