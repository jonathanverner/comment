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
//#include <QtGui/QPointF>

class abstractTool;
class abstractAnnotation;
class QGraphicsItem;
class QEvent;
class pdfPageItem;
struct pdfProperties;

namespace PoDoFo { 
  class PdfDocument;
  class PdfMemDocument;
  class PdfPage;
  class PdfRect;
}

namespace Poppler {
  class Document;
  class TextBox;
}


class pdfCoords;
class pageTextLayer;
class sceneLayer;


/*QRectF Poppler::TextBox::boundingBox() const;
Poppler::TextBox *Poppler::TextBox::nextWord() const;
QString Poppler::TextBox::text() const;*/

struct pageSelections {
	public:
		QList< QList<Poppler::TextBox *> > selections;
		int pageNum;
};


class pdfScene : public QGraphicsScene {
	private:
		QSet<abstractTool *> tools; // a list of registered tools (i.e. annotation types) 
		// This vector is only used while loading a file. Afterwards
		// it is cleared !!!!
		QVector< QList<abstractAnnotation *> > annotations;
		QVector<QPointF> pageCorners; // holds the top left corners of each page
		QVector<pageTextLayer *> textLayer;
		QList<sceneLayer *> sceneLayers;
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

		struct pdfProperties *prop;
		void fillPdfProperties();
		void savePdfProperties( PoDoFo::PdfMemDocument *pdfDoc );
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
		~pdfScene();

		/* Registers an annotation tool */
		void registerTool( abstractTool *tool );

		bool loadFromFile( QString fileName, QObject *pageInViewReceiver = NULL, const char *slot = NULL);
		bool saveToFile( QString fileName );
		bool save();

		void setPdfProperties( struct pdfProperties& properties );
		void getPdfProperties( struct pdfProperties& properties );
	
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

		/****************************************************
		 * GENERAL LAYER FUNCTIONS                          *
		 ****************************************************/

		sceneLayer *addLayer();
		void removeLayer( sceneLayer *layer );

		/****************************************************
		 *             TEXT LAYER METHODS                   *
		 ****************************************************/

		/* Returns a list of wordBoxes containing all the
		 * words to be selected starting from the point @from
		 * to the point @to (both in scene coordinates).
		 * If the rectangle given by (@from,@to) spans
		 * multiple lines, the lines except for the first
		 * and last will be selected entirely, while the first
		 * one will be selected from the @from to the end of the line
		 * and the last will be selected from the beginning of the line to @to.
		 * Currently the granularity is along word boundaries
		 * (i.e. you cannot select a single character) but in the
		 * future this might change. 
		 * Also note that currently selections spanning across
		 * pages are not possible.
		 *
		 * Note: The BBoxes in the returned lists are in page
		 * coordinates of the page containing the point from.
		 *
		 * Note: pdfScene retains ownership of the Poppler::TextBoxes!
		 */
		QList<Poppler::TextBox*> selectText( QPointF from, QPointF to );

		/* Uses the last method to get a list of words and
		 * then concatenates their text content together (i.e. discards
		 * the bbox information) and returns it
		 * both @from and @to are in scene coordinates */

		QString selectedText( QPointF from, QPointF to );

		/* Returns a list of words whose start matches @text,
		 * optionally starting at @startPage (zero-based) and
		 * optionally (if @endPage >=0) ending @endPage
		 *
		 * Note: The BBoxes in the returned lists are in page
		 * coordinates.
		 *
		 * Note: pdfScene retains ownership of the Poppler::TextBoxes!
		 */

		QList< pageSelections > findText( QString text, int startPage = 0, int endPage = -1 );


};
		

#endif /* _pdfScene_H */
