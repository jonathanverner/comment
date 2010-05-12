#ifndef _pdfUtil_H
#define _pdfUtil_H

/**  This file is part of comment
*
*  File: pdfUtil.h
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

#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QRectF>
#include <QtCore/QDebug>

#include <podofo/podofo.h>


class pdfCoords;

namespace Poppler { 
  class TextBox;
}



/* pdfCoords --- translates from the pdfPageItem coordinates (top,left)-based
 *               into the PDF coordinages (bottom, left) based. For this it
 *               needs to know the dimension of the particular page, which
 *               the constructor/setPage method extracts from their parameters
 */
class pdfCoords { 
	private:
		qreal pgSize;
		qreal originX, originY;
	public:
		pdfCoords( PoDoFo::PdfPage *pg = NULL );
		void setPage( PoDoFo::PdfPage *pg );


		/* Note only call the point-methods for
		 * rectangles with zero width and height
		 * otherwise the results need not be what
		 * you expect. */
		QPointF pdfToScene( PoDoFo::PdfRect *pos );
		PoDoFo::PdfRect *sceneToPdf( const QPointF &pos );

		QRectF pdfRectToScene( PoDoFo::PdfRect *pos);
		PoDoFo::PdfRect *sceneToPdf( const QRectF &rect);

};


struct pdfProperties {
  QString author, title, subject, keywords;
};

namespace pdfUtil { 
  PoDoFo::PdfString qStringToPdf( QString str );
  PoDoFo::PdfArray qBoxesToQuadPoints( QList<QRectF> boxes, pdfCoords *coords );
  QList<QRectF> quadPointsToQBoxes( PoDoFo::PdfArray &quadPoints, pdfCoords *coords );
  QString pdfStringToQ( PoDoFo::PdfString str );

  void debugPrintTextBoxen( QList<Poppler::TextBox *> boxen );
  
  PoDoFo::PdfDestination *getDestination( PoDoFo::PdfElement* e );
  QRectF destinationToQRect( PoDoFo::PdfDestination *dest );

};

#endif /* _pdfUtil_H */
