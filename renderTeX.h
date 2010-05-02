#ifndef _renderTeX_H
#define _renderTeX_H

/**  This file is part of comment
*
*  File: renderTeX.h
*  Created: 17. 11. 2008
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


#include <QtCore/QVector>
#include <QtCore/QStack>
#include <QtCore/QCache>
#include <QtCore/QString>
#include <QtGui/QPixmap>

class renderItem;

class renderTeX : public QObject { 
  Q_OBJECT
	private:
		struct cachedPage { 
		  bool format_inline;
		  qreal zoom;
		  QPixmap pix;
		};
		static QCache<int, struct cachedPage> renderCache;
		QVector<renderItem*> items;
		QStack<int> available_ids;

		QString preambule;

	protected slots:
		void renderingFinished( int i );

	public:
		renderTeX( QString preamb="" );

		void setPaths( QString pdfLaTeX, QString ghostScript );
		void setPreambule( QString preambule );

		/* This method should always succeed */
		int addItem( QString source, QString preambule = "" );
		void setItem( int itemID, QString source, QString preambule = "" );
		void updateItem( int item, QString source, QString preambule = "" );
		void deleteItem( int item );
		// sizeHint is the wanted width in millimeters
		QPixmap render( int item, bool format_inline = false, qreal zoom = 1, int sizeHint = 50 );
		void preRender( int item, bool format_inline = false, int sizeHint = 50 );
		QString getPDF( int item );
		QRectF getBBox( int item );
	signals:
		void itemReady( int item );


		
};


#endif /* _renderTeX_H */
