#ifndef _wordItem_H
#define _wordItem_H

/***************************************************************
 * wordItem.h
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
#include <QtGui/QGraphicsItem>

namespace Poppler {
  class TextBox;
}


class wordItem : public QGraphicsItem { 
	private:
		Poppler::TextBox *bx;
		bool hilited;
		qreal w,h;
	public:
		wordItem( Poppler::TextBox *box );
		void hilight(bool on = true);

		QString word();
		QRectF boundingRect() const;
		void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget );

};




#endif /* _wordItem_H */
