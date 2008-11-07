#ifndef _textLayer_H
#define _textLayer_H

/***************************************************************
 * textLayer.h
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

#include <QtCore/QVector>
#include <QtCore/QList>
#include <QtGui/QGraphicsItem>

namespace Poppler { 
  class Page;
}

class line;

class textLayer : public QGraphicsItem { 
	private:
		QVector<line*> lines;

		int findLine( qreal y );

	public:
		textLayer( Poppler::Page *pg );

		QList<QRectF> select( QPointF from, QPointF to );
		QRectF boundingRect() const { return QRectF(0,0,0,0);};
		void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget ) {};


};



#endif /* _textLayer_H */
