#ifndef _pagTextLayer_H
#define _pageTextLayer_H

/***************************************************************
 * pageTextLayer.h
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

#include "pdfScene.h"

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
