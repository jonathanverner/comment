/***************************************************************
 * pdfPageItem.cpp
 * @Author:      Jonathan Verner (jonathan.verner@matfyz.cz)
 * @License:     GPL v2.0 or later
 * @Created:     2008-10-12.
 * @Last Change: 2008-10-12.
 * @Revision:    0.0
 * Description:
 * Usage:
 * TODO:
 *CHANGES:
 ***************************************************************/
#include <QtGui/QPainter>
#include <QtGui/QImage>
#include <QtGui/QStyleOptionGraphicsItem>
#include <QtCore/QDebug>


#include <poppler-qt4.h>

#include "pdfPageItem.h"

pdfPageItem::pdfPageItem( Poppler::Page *page ) : pdfPage(page) {
}

pdfPageItem::~pdfPageItem() {
  delete pdfPage;
}

QRectF pdfPageItem::boundingRect() const { 
  QSizeF sz = pdfPage->pageSizeF();
  return QRectF( 0, 0, sz.width(), sz.height() );
}

void pdfPageItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget ) {
  qreal zoom = option->levelOfDetail;
  QRectF exposed = option->exposedRect;
  qreal x,y,w,h;
  option->exposedRect.getRect( &x, &y, &w, &h );
  QImage image = pdfPage->renderToImage( 72*zoom, 72*zoom, (int) (x*zoom), (int) (y*zoom), (int) (w*zoom), (int) (h*zoom) );
  painter->drawImage( exposed, image );
}


