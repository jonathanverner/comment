/***************************************************************
 * pdfUtil.cpp
 * @Author:      Jonathan Verner (jonathan.verner@matfyz.cz)
 * @License:     GPL v2.0 or later
 * @Created:     2008-11-07.
 * @Last Change: 2008-11-07.
 * @Revision:    0.0
 * Description:
 * Usage:
 * TODO:
 *CHANGES:
 ***************************************************************/

#include <QtCore/QByteArray>
#include "pdfUtil.h"
#include "pdfScene.h"

using namespace PoDoFo;



pdfCoords::pdfCoords( PdfPage *pg ) { 
  if ( pg ) setPage( pg );
  else pgSize = 0;
}

void pdfCoords::setPage( PdfPage *pg ) { 
  if ( pg ) pgSize = pg->GetPageSize().GetHeight();
  else {
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
  QPointF ret( (pos->GetLeft()-pos->GetWidth())-8, (pgSize-pos->GetBottom()-pos->GetHeight())-8 );
  return ret;
}

PdfRect *pdfCoords::sceneToPdf( const QPointF &pos ) { 
  return new PdfRect( pos.x()+8, (pgSize-pos.y())-8, 0, 0 );
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
void addBox( PdfArray &array, QRectF &box, pdfCoords *coords ) { 
    PdfRect *rectBL, *rectTR;
    rectBL = coords->sceneToPdf( box.bottomLeft() );
    rectTR = coords->sceneToPdf( box.topRight() );
    // BL
    array.push_back( PdfVariant( rectBL->GetLeft() ) );
    array.push_back( PdfVariant( rectBL->GetBottom() ) );
    // BR
    array.push_back( PdfVariant( rectTR->GetLeft() ) );
    array.push_back( PdfVariant( rectBL->GetBottom() ) );
    //TL
    array.push_back( PdfVariant( rectBL->GetLeft() ) );
    array.push_back( PdfVariant( rectTR->GetBottom() ) );
    // TR
    array.push_back( PdfVariant( rectTR->GetLeft() ) );
    array.push_back( PdfVariant( rectTR->GetBottom() ) );

    delete rectBL;
    delete rectTR;
}

QRectF getBox( std::vector<PdfObject>::iterator &pos, pdfCoords *coords ) { 
  PdfVariant x1(*pos);
  pos++;
  PdfVariant y1(*pos);
  pos++;
  PdfVariant x2(*pos);
  pos++;
  PdfVariant y2(*pos);
  pos++;
  PdfVariant x3(*pos);
  pos++;
  PdfVariant y3(*pos);
  pos++;
  PdfVariant x4(*pos);
  pos++;
  PdfVariant y4(*pos);
  pos++;
  PdfRect bl( x1.GetReal(), y1.GetReal(),0,0 ), tr( x4.GetReal(), y4.GetReal(),0,0 );
  qDebug() << "PDFBOX: (" << x1.GetReal() <<","<<y1.GetReal()<<","<<x4.GetReal()-x1.GetReal()<<","<<y4.GetReal()-y1.GetReal()<<")";
  return QRectF( coords->pdfToScene( &bl ), coords->pdfToScene( &tr ) );
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


