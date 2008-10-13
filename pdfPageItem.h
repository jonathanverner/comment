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

class Poppler::Page;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;

class pdfPageItem : public QGraphicsItem { 
	private:
		Poppler::Page *pdfPage;
	public:
		pdfPageItem( Poppler::Page *page );
		~pdfPageItem();
		QRectF boundingRect() const;
		void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget );
};

#endif // PDFPAGEITEM_H
