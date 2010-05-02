#ifndef _pageBeginItem_H
#define _pageBeginItem_H


/**  This file is part of comment
*
*  File: pageEndItem.h
*  Created: 17. 10. 2008
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
		// pgNum page numbers start from 1
		pageBeginItem( int pgNum, qreal wd, QGraphicsItem *parent = NULL );
		~pageBeginItem();
		QRectF boundingRect() const { return QRectF(0,0,wd,10); };
		pageBeginSignalEmitter *getSignalEmitter() { return sg; };
		void setPageNum( int pgNum ) {pageNum = pgNum;}
		int getPageNum() const { return pageNum;};
		void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget );
};


#endif /* _pageBeginItem_H */
