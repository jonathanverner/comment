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


