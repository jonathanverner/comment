/**  This file is part of project comment
 *
 *  File: pdfPageItem.cpp
 *  Created: 2008-10-12
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




#include <poppler-qt4.h>

#include "pdfPageItem.h"

#include <QtGui/QPainter>
#include <QtGui/QImage>
#include <QtGui/QStyleOptionGraphicsItem>
#include <QtCore/QDebug>


QCache<int, struct pdfPageItem::cachedPage> pdfPageItem::renderCache(200);

pdfPageItem::~pdfPageItem() {
  delete pdfPage;
}

pdfPageItem::pdfPageItem( Poppler::Page *page ) : pdfPage( page ) {
};

QRectF pdfPageItem::boundingRect() const { 
  QSizeF sz = pdfPage->pageSizeF();
  return QRectF( 0, 0, sz.width(), sz.height() );
}


void pdfPageItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget ) {
#if QT_VERSION < 0x040600
  qreal zoom = option->levelOfDetail;
#else
  qreal zoom = option->levelOfDetailFromTransform( painter->worldTransform() );
#endif
  cachedPage *page = renderCache.object( pageNum );
  QPixmap pix;
  
  if ( ! page ) pix = populateCache( zoom );
  else if ( zoom != page->zoom ) { 
    renderCache.remove( pageNum );
    pix = populateCache( zoom );
  } else pix = page->pix;
  QRectF exposed = option->exposedRect;
  qreal x,y,w,h;
  option->exposedRect.getRect( &x, &y, &w, &h );
  painter->drawPixmap( exposed, pix, QRectF( x*zoom,y*zoom,w*zoom,h*zoom ) );

/*  QRectF exposed = option->exposedRect;
  qreal x,y,w,h;
  option->exposedRect.getRect( &x, &y, &w, &h );
  QImage image = pdfPage->renderToImage( 72*zoom, 72*zoom, (int) (x*zoom), (int) (y*zoom), (int) (w*zoom), (int) (h*zoom) );
  painter->drawImage( exposed, image ); */
}

QPixmap pdfPageItem::populateCache( qreal zoom ) { 
  qDebug() << "Populating cache for zoom "<< zoom;
  QImage image = pdfPage->renderToImage( 72*zoom, 72*zoom );
  cachedPage *page = new cachedPage;
  page->pix = QPixmap::fromImage( image );
  page->zoom = zoom;
  renderCache.insert( pageNum, page, (int) (zoom*10) );
  return page->pix;
}
  
