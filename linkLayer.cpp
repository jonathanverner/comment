/**  This file is part of project comment
 *
 *  File: linkLayer.cpp
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


#include "linkLayer.h"
#include "pdfScene.h"


#include <QtCore/QSignalMapper>
#include <QAction>

void targetItem::activate()
{
  emit activated();
}




linkLayer::linkLayer(pdfScene* sc):
  sceneLayer(sc)
{
  mapper = new QSignalMapper( this );
  connect( mapper, SIGNAL(mapped(const QString &)), this, SLOT(emitGOTO(const QString &)) );
}

targetItem* linkLayer::addTarget ( const QString& name, const QRectF& target ) {
  if ( targets.contains( name ) ) return targets[name];
  QRectF area = QRectF(QPointF(0,0),target.size());
  targetItem *tgt = new targetItem( area, name );
  connect( tgt, SIGNAL(activated()), mapper, SLOT(map()));
  mapper->setMapping( tgt, name );
  targets.insert( name, tgt );
  addItem( tgt );
  tgt->setPos( target.topLeft() );
}

targetItem* linkLayer::addTarget ( const QString& name, const int page, const QRectF& target )
{
  if ( targets.contains( name ) ) return targets[name];
  QRectF area = QRectF(QPointF(0,0),target.size());
  targetItem *tgt = new targetItem( target, name );
  connect( tgt, SIGNAL(activated()), mapper, SLOT(map()));
  mapper->setMapping( tgt, name );
  targets.insert( name, tgt );
  addItem( tgt );
  pdfScene *sc = dynamic_cast<pdfScene *>(scene);
  tgt->setPos(target.topLeft()+sc->topLeftPage(page));
}

void linkLayer::removeTarget ( const QString& name ) {
  if ( ! targets.contains( name ) ) return;
  targetItem *tgt = targets[name];
  delete tgt;
  targets.remove(name);
}



void linkLayer::emitGOTO(const QString& name) {
  if ( targets.contains( name ) ) emit gotoTarget( targets[name] );
}



#include "linkLayer.moc"
