#ifndef PDFPAGEITEM_H
#define PDFPAGEITEM_H

/**  This file is part of comment
*
*  File: pdfPageItem.h
*  Created: 12. 10. 2008
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

#include <QtGui/QGraphicsItem>
#include <QtCore/QRectF>
#include <QtCore/QCache>

namespace Poppler {
  class Page;
};

class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;


class pdfPageItem : public QGraphicsItem { 
	private:
		Poppler::Page *pdfPage;
		int pageNum;
		struct cachedPage { 
		  qreal zoom;
		  QPixmap pix;
		};
		static QCache<int, struct cachedPage> renderCache;
		QPixmap populateCache( qreal zoom );
	public:
		pdfPageItem( Poppler::Page *page );
		~pdfPageItem();

		Poppler::Page *getPage() { return pdfPage; }
		QRectF boundingRect() const;
		void setPageNum( int pgNum ) {pageNum = pgNum;}
		int getPageNum() const { return pageNum;};
		void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget );

};

#endif // PDFPAGEITEM_H
