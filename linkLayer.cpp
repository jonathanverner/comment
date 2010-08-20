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

#include <QtCore/QDebug>

#include <podofo/podofo.h>
#include "pdfUtil.h"

QString linkLayer::generateName() {
  return "generated."+QString::number(generation++);
}

linkLayer::linkLayer(pdfScene* sc):
  sceneLayer(sc), generation(0)
{
  connect( sc, SIGNAL(finishedLoading()), this, SLOT(placeOnPages()) );

}

void linkLayer::loadFromDoc(PoDoFo::PdfMemDocument* doc) {
  qDebug() << "linkLayer: Loading named destinations ...";
  try {
    PoDoFo::PdfNamesTree* pNames = doc->GetNamesTree( PoDoFo::ePdfDontCreateObject );
    if( ! pNames ) return;
    PoDoFo::PdfDictionary destsDict;
    pNames->ToDictionary( PoDoFo::PdfName("Dests"), destsDict );
    PoDoFo::TKeyMap keyMap = destsDict.GetKeys();
    QString tName;
    PoDoFo::PdfDestination *dest;
    PoDoFo::PdfObject *obj;
    for(PoDoFo::TKeyMap::const_iterator it = keyMap.begin(); it != keyMap.end(); ++it ) {
      try {
	tName = QString::fromUtf8( it->first.GetName().c_str() );
	qDebug() << "Processing "<< tName;
        obj = pdfUtil::resolveRefs( doc, it->second );
	if ( obj->IsArray() ) dest = new PoDoFo::PdfDestination( obj );
	else if ( obj->IsDictionary() ) {
	  obj->GetDictionary().GetKey("D")->SetOwner( &doc->GetObjects() );
	  dest = new PoDoFo::PdfDestination( obj->GetDictionary().GetKey("D") );
	}
	else {
	  qDebug() << "Element is neither an array, nor a dictionary:"<< obj->GetDataTypeString();
	  continue;
	}
	addTarget( tName, dest );
      } catch ( PoDoFo::PdfError e ) {
	qDebug() << "linkLayer: Error adding named destination ("<<tName<<"):"<<e.what();
      }
    }
  } catch ( PoDoFo::PdfError  e ) {
    qDebug() << "linkLayer: Error processing names tree:" << e.what();
  };
  qDebug() << "linkLayer: Done loading named destinations.";
}


void linkLayer::saveToDoc(PoDoFo::PdfDocument* doc) {
  try {
    PoDoFo::PdfNamesTree* pNames = doc->GetNamesTree( PoDoFo::ePdfCreateObject );
    if( ! pNames ) { 
      qDebug() << "Error retrieving/creating names tree";
      return;
    }
    PoDoFo::PdfPage *pg;
    PoDoFo::PdfDestination *dest;
    foreach( targetItem *tgt, targets ) {
      pg = doc->GetPage(tgt->getPage());
      dest = tgt->getPdfDest(pg);
      doc->AddNamedDestination( *dest, pdfUtil::qStringToPdf(tgt->getName()));
      delete dest;      
    };
  } catch ( PoDoFo::PdfError  e ) {
    qDebug() << "linkLayer: Error processing names tree:" << e.what();
  };
}


targetItem* linkLayer::addTarget ( QString& name, const int page, const QRectF& target )
{
  if ( name == "" ) name = generateName();
  if ( targets.contains( name ) ) {
    qDebug() << "Not replacing target named: " << name;
    return targets[name];
  }
  targetItem *tgt = new targetItem( page, target.size(), target.topLeft(), name );
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
      tgt->setPos(tgt->getPagePos()+sc->topLeftPage(tgt->getPage()));
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
  targetItem *tgt = new targetItem( page, tgtRect.size(), tgtRect.topLeft(), name );
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



PoDoFo::PdfDestination *targetItem::getPdfDest(PoDoFo::PdfPage* pg) {
  pdfCoords transform( pg );
  PoDoFo::PdfRect *rect = transform.sceneToPdf( brect.translated(pgPos) );
  PoDoFo::PdfDestination *ret = new PoDoFo::PdfDestination( pg, *rect );
  //ret->GetObject()->GetDictionary().AddKey(PoDoFo::PdfName("comment_target_name"),pdfUtil::qStringToPdf(name));
  delete rect;
  return ret;
}



#include "linkLayer.moc"
