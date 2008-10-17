#ifndef _pageBeginItem_H
#define _pageBeginItem_H

/***************************************************************
 * pageEndItem.h
 * @Author:      Jonathan Verner (jonathan.verner@matfyz.cz)
 * @License:     GPL v2.0 or later
 * @Created:     2008-10-17.
 * @Last Change: 2008-10-17.
 * @Revision:    0.0
 * Description:
 * Usage:
 * TODO:
 *CHANGES:
 ***************************************************************/
#include <QtGui/QGraphicsItem>

class pageBeginSignalEmitter : public QObject { 
  Q_OBJECT
	public:
		void emitSignal( int pg ) { emit pageInView( pg ); }
	signals:
		void pageInView( int pg );
};

class pageBeginItem : public QGraphicsItem {
	private:
		int pageNum;
		qreal wd;
		pageBeginSignalEmitter *sg;
	public:
		pageBeginItem( int pgNum, qreal wd );
		~pageBeginItem();
		QRectF boundingRect() const { return QRectF(0,0,wd,10); };
		pageBeginSignalEmitter *getSignalEmitter() { return sg; };
		void setPageNum( int pgNum ) {pageNum = pgNum;}
		int getPageNum() const { return pageNum;};
		void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget );
};


#endif /* _pageBeginItem_H */
