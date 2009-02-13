#ifndef _hiliteItem_H
#define _hiliteItem_H

/***************************************************************
 * hiliteItem.h
 * @Author:      Jonathan Verner (jonathan.verner@matfyz.cz)
 * @License:     GPL v2.0 or later
 * @Created:     2009-02-13.
 * @Last Change: 2009-02-13.
 * @Revision:    0.0
 * Description:
 * Usage:
 * TODO:
 *CHANGES:
 ***************************************************************/
#include <QtGui/QGraphicsItem>
#include <QtGui/QPainter>
#include <QtGui/QPainterPath>
#include <QtGui/QColor>

namespace Poppler { 
  class TextBox;
};

class hiliteItem : public QGraphicsItem {
	private:
		QList<QRectF> hBoxes;
		QRectF bBox;
		QPainterPath exactShape;
		QColor col;
		bool active;

	public:
		hiliteItem();
		hiliteItem( QPointF topLeftPage, QList<Poppler::TextBox *> bboxes );

		void updateBBoxes( QList<Poppler::TextBox*> bboxes );
		void clear();

		void setColor( QColor col );
		void setActive();
		void setInactive();

		virtual QRectF boundingRect() const;

		void paint( QPainter *, const QStyleOptionGraphicsItem *, QWidget * );
		QPainterPath shape() const;
};


#endif /* _hiliteItem_H */
