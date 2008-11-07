#ifndef _pdfScene_H
#define _pdfScene_H

/***************************************************************
 * pdfScene.h
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


#include <QtGui/QGraphicsScene>
#include <QtCore/QString>
#include <QtCore/QVector>
#include <QtCore/QList>

class abstractTool;
class abstractAnnotation;
class QGraphicsItem;
class QEvent;
class pdfPageItem;

namespace PoDoFo { 
  class PdfDocument;
  class PdfPage;
  class PdfRect;
}

namespace Poppler {
  class Document;
}

/* pdfCoords --- translates from the pdfPageItem coordinates (top,left)-based
 *               into the PDF coordinages (bottom, left) based. For this it
 *               needs to know the dimension of the particular page, which
 *               the constructor/setPage method extracts from their parameters
 */
class pdfCoords { 
	private:
		qreal pgSize;
	public:
		pdfCoords( PoDoFo::PdfPage *pg = NULL );
		void setPage( PoDoFo::PdfPage *pg );


		QPointF pdfToScene( PoDoFo::PdfRect *pos );
		PoDoFo::PdfRect *sceneToPdf( const QPointF &pos );
		PoDoFo::PdfRect *sceneToPdf( const QRectF &rect );
};


class pdfScene : public QGraphicsScene {
	private:
		QSet<abstractTool *> tools; // a list of registered tools (i.e. annotation types) 
		// This vector is only used while loading a file. Afterwards
		// it is cleared !!!!
		QVector< QList<abstractAnnotation *> > annotations;
		QVector<QPointF> pageCorners; // holds the top left corners of each page
		qreal pageSkip; // amount of space to be left between the pages of the pdf
		qreal leftSkip; // the left margin
		QString myFileName; // the filename of the file currently opened
		/* The temporary file:
		 *    When we load a pdf, we first load it via PoDoFo and process
		 *    the annotations. Annotations which are recognized by some
		 *    registered tool are then deleted from the document. The
		 *    resulting document is saved to the temporary file and
		 *    this temporary file is loaded by poppler for the rendering.
		 *    This ensures that poppler gets a chance to render annotations
		 *    which are not supported by us.
		 *    Saving works by loading the temporary file via PoDoFo, adding
		 *    the annotations and writing out the file to myFileName (or a
		 *    filename provided to the saveToFile method */
		QByteArray tempFileName; // the temporary file 
		int numPages; // number of pages;

		Poppler::Document *pdf; // When a document is loaded, this holds the poppler document

		void processPage( PoDoFo::PdfDocument *pdf, int pgNum ); //
		void loadPopplerPdf( QString fileName, QObject *pageInViewReceiver, const char *slot );
		void mergeAnnotationsFromPage( PoDoFo::PdfDocument *pdf, int pgNum );
		void generateTempFileName();
		void addPageAnnotations( int pageNum, QGraphicsItem *pageItem );

		pdfPageItem *getPageItem( int pgNum );


	public:
		pdfScene();
		pdfScene( const QSet<abstractTool *> &tools, QString fileName = "");

		/* Registers an annotation tool */
		void registerTool( abstractTool *tool );

		bool loadFromFile( QString fileName, QObject *pageInViewReceiver = NULL, const char *slot = NULL);
		bool saveToFile( QString fileName );
		bool save();

	
		/* Info Stuff */
		int getNumPages() { return numPages; };

		/* Always retuns a valid page number in [0,numPages) */
		int posToPage( const QPointF &scenePos ); 
		
		/* Returns the Scene coordinates of the top left corner
		 * of page page. If page is not a valid page number
		 * (zero based, i.e. in the interval [0,numPages) )
		 * then returns (0,0) */
		QPointF topLeftPage( int page );

		/* Places the annotation annot ( which must not be NULL )
		 * on the page determined by the scene position scPos */
		void placeAnnotation( abstractAnnotation *annot, const QPointF *scPos ); 
};
		

#endif /* _pdfScene_H */
