#ifndef _pdfUtil_H
#define _pdfUtil_H

/***************************************************************
 * pdfUtil.h
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




namespace pdfUtil { 

  PoDoFo::PdfString qStringToPdf( QString str );
  PoDoFo::PdfArray qBoxesToQuadPoints( QList<QRectF> boxes, pdfCoords *coords );
  QList<QRectF> quadPointsToQBoxes( PoDoFo::PdfArray &quadPoints, pdfCoords *coords );
  QString pdfStringToQ( PoDoFo::PdfString str );

  void debugPrintTextBoxen( QList<Poppler::TextBox *> boxen );


}

#endif /* _pdfUtil_H */
