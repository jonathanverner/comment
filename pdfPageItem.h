#ifndef PDFPAGEITEM_H
#define PDFPAGEITEM_H

/***************************************************************
 * pdfItem.cpp
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

#include <QtGui/QGraphicsItem>
#include <QtCore/QRectF>
#include <QtCore/QCache>

class Poppler::Page;
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
