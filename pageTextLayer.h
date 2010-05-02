#ifndef _pageTextLayer_H
#define _pageTextLayer_H

/**  This file is part of comment
*
*  File: pageTextLayer.h
*  Created: 7. 11. 2008
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
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QPointF>

namespace Poppler { 
  class Page;
  class TextBox;
}

class line;

class pageTextLayer { 
	private:
		QVector<line*> lines;
		QString pageText;

//		int findLine( qreal y );
		template <class T> int findLine( T pos, int minLineHint=0 );
		QList<Poppler::TextBox*> interval( int startPos, int endPos );

	public:
		pageTextLayer( Poppler::Page *pg );
		~pageTextLayer();

		/* pageTextLayer retains ownership of
		 * the returned TextBoxes. So do not
		 * delete them yourself and do not dereference
		 * the pointers after deleting pageTextLayer !!! */

		QList<Poppler::TextBox*> select( QPointF from, QPointF to );
		QList< QList<Poppler::TextBox*> > findText( QString text );


};



#endif /* _pageTextLayer_H */
