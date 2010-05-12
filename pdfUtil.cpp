/**  This file is part of project comment
 *
 *  File: pdfUtil.cpp
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




#include "pdfUtil.h"
#include "pdfScene.h"

#include <QtCore/QByteArray>
#include <QtCore/QDebug>

#include <poppler-qt4.h>

using namespace PoDoFo;
using namespace Poppler;



pdfCoords::pdfCoords( PdfPage *pg ) { 
  if ( pg ) setPage( pg );
  else pgSize = 0;
}

void pdfCoords::setPage( PdfPage *pg ) { 
  if ( pg ) {
    pgSize = pg->GetPageSize().GetHeight();
    originX = pg->GetCropBox().GetLeft();
    originY = pg->GetCropBox().GetBottom();
  } else {
    pgSize = 0;
    qWarning() << "pdfCoords: Warning, calling setPage with a null pointer;";
  }
}

/* 
 * FIXME: PDF<->QGraphicsScene coordinate conversion
 *        needs to be fixed. !!!! 
 *        It's weird that when saving, we need to subtract
 *        from y as we also do when loading. This should
 *        gradually accumulate, but it isn't happening,
 *        so there is a bug somewhere. We only need to
 *        find it :-(
 * */
QPointF pdfCoords::pdfToScene( PdfRect *pos ) { 
  QPointF ret( pos->GetLeft()-originX, pgSize-(pos->GetBottom()+originY) );
  return ret;
}

PdfRect *pdfCoords::sceneToPdf( const QPointF &pos ) { 
  return new PdfRect( pos.x()+originX, (pgSize-pos.y())-originY, 0, 0 );
}

PdfRect *pdfCoords::sceneToPdf( const QRectF &rect ) { 
  return new PdfRect( rect.x()+originX, ( pgSize-(rect.y()+rect.height()) )-originY, rect.width(), rect.height() );
}

QRectF pdfCoords::pdfRectToScene( PdfRect *rect ) {
  QRectF ret( rect->GetLeft()-originX, ( pgSize-(rect->GetBottom()+originY+rect->GetHeight()) ), rect->GetWidth(), rect->GetHeight() );
  return ret;
}

PdfString pdfUtil::qStringToPdf( QString str ) { 
  QByteArray tmp = str.toUtf8();
  return PdfString( (pdf_utf8*) tmp.constData() );
}

QString pdfUtil::pdfStringToQ( PdfString str ) { 
  std::string tmp = str.GetStringUtf8();
  return QString::fromUtf8( tmp.c_str() );
}

namespace pdfUtil {

void debugPrintTextBoxen( QList<TextBox *> boxen ) { 
  QString print;
  int sz=0;
  foreach( TextBox *b, boxen ) { 
    print = print + " + " + b->text();
    sz+=b->text().size()+1;
  }
  qDebug() << print << "(size:"<<sz<<")";
}


void addBox( PdfArray &array, QRectF &box, pdfCoords *coords ) { 
    PdfRect *rect = coords->sceneToPdf( box );

    // TL
    array.push_back( PdfVariant( rect->GetLeft() ) );
    array.push_back( PdfVariant( rect->GetBottom()+rect->GetHeight() ) );

    //TR
    array.push_back( PdfVariant( rect->GetLeft()+rect->GetWidth() ) );
    array.push_back( PdfVariant( rect->GetBottom()+rect->GetHeight() ) );

    // BL
    array.push_back( PdfVariant( rect->GetLeft() ) );
    array.push_back( PdfVariant( rect->GetBottom() ) );
    // BR
    array.push_back( PdfVariant( rect->GetLeft()+rect->GetWidth() ) );
    array.push_back( PdfVariant( rect->GetBottom() ) );

    delete rect;
}

QRectF getBox( std::vector<PdfObject>::iterator &pos, pdfCoords *coords ) { 
  PdfVariant x4(*pos);
  pos++;
  PdfVariant y4(*pos);
  pos++;
  PdfVariant x3(*pos);
  pos++;
  PdfVariant y3(*pos);
  pos++;
  PdfVariant x1(*pos);
  pos++;
  PdfVariant y1(*pos);
  pos++;
  PdfVariant x2(*pos);
  pos++;
  PdfVariant y2(*pos);
  pos++;
  PdfRect rect( x1.GetReal(), y1.GetReal(),x2.GetReal()-x1.GetReal(),y4.GetReal()-y1.GetReal() );
  return  coords->pdfRectToScene( &rect );
}


}




PdfArray pdfUtil::qBoxesToQuadPoints( QList<QRectF> boxes, pdfCoords *coords ) {
  PdfArray ret;
  foreach( QRectF box, boxes ) { 
    pdfUtil::addBox( ret, box, coords );
  }
  return ret;
}

QList<QRectF> pdfUtil::quadPointsToQBoxes( PdfArray &quadPoints, pdfCoords *coords ) {
  QList<QRectF> ret;
  std::vector<PdfObject>::iterator pos = quadPoints.begin(), e = quadPoints.end();
  while( pos < e )
    ret.append( pdfUtil::getBox( pos, coords ) );
  return ret;
}

PdfObject* resolveRefs( PdfMemDocument *doc, PdfObject *ref )  {
  PdfObject *ret = ref;
  while ( ret->IsReference() ) {
    ret = doc->GetObjects().GetObject(ret->GetReference());
    if ( ! ret ) return NULL;
  }
  return ret;
}


PdfDestination* pdfUtil::getDestination(PdfMemDocument *doc, PdfElement* e) {
  PdfDestination *ret = NULL;
  PdfOutlineItem *outLineItem = dynamic_cast<PdfOutlineItem*>(e);
  PdfAnnotation *linkAnnot = dynamic_cast<PdfAnnotation*>(e);
  if ( ! linkAnnot && ! outLineItem ) return ret;
  try {
  if ( outLineItem ) ret = outLineItem->GetDestination();
  else if ( linkAnnot ) ret = new PdfDestination(linkAnnot->GetDestination()); // Memory leak ??
  if ( ret ) return ret;
  PdfObject *a = e->GetObject()->GetDictionary().GetKey(PdfName("A"));
  a = resolveRefs( doc, a );
  if ( ! a ) {
    qDebug() << "getDestination: Reference not found";
    return NULL;
  }
  PdfDictionary s = a->GetDictionary();
  if ( s.GetKey(PdfName("S"))->GetName() == PdfName("GoTo") ) {
    PdfObject *d = a->GetDictionary().GetKey(PdfName("D"));
    d->SetOwner(&doc->GetObjects());
    ret  = new PdfDestination( d );
  }
  } catch ( PdfError e ) {
    qDebug() << e.what();
  }
  return ret;
}

QRectF pdfUtil::destinationToQRect(PdfDestination* dest) {
  try {
    pdfCoords transform( dest->GetPage() );
    PdfArray dst = dest->GetArray();
    PdfName tp = dst[1].GetName();
    QRectF ret;
    PdfRect pRect;
    if ( tp == PdfName("XYZ" ) ) { 
      PdfRect rect(dst[2].GetReal(),dst[3].GetReal(),0,0);
      return transform.pdfRectToScene( &rect );
    } else if ( tp == PdfName("FitR") ) {
      PdfRect rect(dst[2].GetReal(),dst[3].GetReal(),dst[4].GetReal(),dst[5].GetReal());
      return transform.pdfRectToScene(&rect);
    } else if ( tp == PdfName("FitH") ) {
      PdfRect rect(0,dst[2].GetReal(),0,0);
      return transform.pdfRectToScene(&rect);
    } else if ( tp == PdfName("FitV") ) {
       return QRectF( 0,dst[2].GetReal(),0,0 );
    }
  } catch (PdfError e) {
    qDebug() << e.what();
  }
 return QRectF(0,0,0,0);
};
