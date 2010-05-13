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
#include <QtGui/QAction>
#include <QtCore/QDebug>

#include <podofo/podofo.h>
#include "pdfUtil.h"

QString linkLayer::generateName() {
  return "generated."+QString::number(generation++);
}


void targetItem::activate()
{
  emit activated();
}




linkLayer::linkLayer(pdfScene* sc):
  sceneLayer(sc), generation(0)
{
  mapper = new QSignalMapper( this );
  connect( mapper, SIGNAL(mapped(const QString &)), this, SLOT(emitGOTO(const QString &)) );
  connect( sc, SIGNAL(finishedLoading()), this, SLOT(placeOnPages()) );
}


targetItem* linkLayer::addTarget ( QString& name, const int page, const QRectF& target )
{
  if ( name == "" ) name = generateName();
  if ( targets.contains( name ) ) {
    qDebug() << "Not replacing target named: " << name;
    return targets[name];
  }
  targetItem *tgt = new targetItem( page, target.size(), name );
  connect( tgt, SIGNAL(activated()), mapper, SLOT(map()));
  mapper->setMapping( tgt, name );
  targets.insert( name, tgt );
  addItem( tgt );
  pdfScene *sc = dynamic_cast<pdfScene *>(scene);
  tgt->setPos(target.topLeft()+sc->topLeftPage(page));
  qDebug() << "Target: " << name << "Position:" << target << " (on page " << page << ")";
  return tgt;
}

void linkLayer::placeOnPages() {
  targetItem *tgt;
  pdfScene *sc = dynamic_cast<pdfScene *>(scene);
  foreach( QGraphicsItem *item, items ) {
    if ( (tgt = dynamic_cast<targetItem*>(item)) ) {
      tgt->setPos(tgt->pos()+sc->topLeftPage(tgt->getPage()));
    }
  }
}


targetItem* linkLayer::addTarget(QString& name, PoDoFo::PdfDestination* dest) {
  if ( name == "" ) name = generateName();
  if ( targets.contains( name ) ) {
    qDebug() << "Not replacing target named: " << name;
    return targets[name];
  }
  QRectF tgtRect = pdfUtil::destinationToQRect( dest );
  int page = dest->GetPage()->GetPageNumber()-1;
  targetItem *tgt = new targetItem( page, tgtRect.size() , name );
  connect( tgt, SIGNAL(activated()), mapper, SLOT(map()));
  mapper->setMapping( tgt, name );
  targets.insert( name, tgt );
  addItem( tgt );
  pdfScene *sc = dynamic_cast<pdfScene *>(scene);
  tgt->setPos(tgtRect.topLeft()+sc->topLeftPage(page));
  qDebug() << "Target: " << name << "Position:" << tgtRect << " (on page " << page << ", "<<sc->topLeftPage(page)<<")";
  return tgt;
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

PoDoFo::PdfDestination *targetItem::getPdfDest(PoDoFo::PdfPage* pg) {
  pdfCoords transform( pg );
  PoDoFo::PdfRect *rect = transform.sceneToPdf( brect );
  PoDoFo::PdfDestination *ret = new PoDoFo::PdfDestination( pg, *rect );
  ret->GetObject()->GetDictionary().AddKey(PoDoFo::PdfName("comment_target_name"),pdfUtil::qStringToPdf(name));
  delete rect;
  return ret;
}



#include "linkLayer.moc"
