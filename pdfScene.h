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

namespace PoDoFo { 
  class PdfDocument;
  class PdfPage;
  class PdfRect;
}

namespace Poppler {
  class Document;
}

class pdfCoords { 
	private:
		qreal pgSize;
	public:
		pdfCoords( PoDoFo::PdfPage *pg );
		QPointF pdfToScene( PoDoFo::PdfRect *pos );
		PoDoFo::PdfRect *sceneToPdf( const QPointF &pos );
		PoDoFo::PdfRect *sceneToPdf( const QRectF &rect );
};


class pdfScene : public QGraphicsScene {
	private:
		QSet<abstractTool *> tools;
		QVector< QList<abstractAnnotation *> > annotations;
		QVector<QPointF> pageCorners;
		qreal pageSkip; // amount of space to be left between the pages of the pdf
		qreal leftSkip; // the left margin
		QString myFileName;
		QByteArray tempFileName;
		int numPages;

		Poppler::Document *pdf;

		void processPage( PoDoFo::PdfDocument *pdf, int pgNum );
		void loadPopplerPdf( QString fileName, QObject *pageInViewReceiver, const char *slot );
		void mergeAnnotationsFromPage( PoDoFo::PdfDocument *pdf, int pgNum );
		void generateTempFileName();
		void addPageAnnotations( int pageNum, QGraphicsItem *pageItem );

	public:
		pdfScene();
		pdfScene( const QSet<abstractTool *> &tools, QString fileName = "");

		void registerTool( abstractTool *tool );

		bool loadFromFile( QString fileName, QObject *pageInViewReceiver = NULL, const char *slot = NULL);
		bool saveToFile( QString fileName );
		bool save();

		int getNumPages() { return numPages; };

		/* Always retuns a valid page number in [0,numPages) */
		int posToPage( const QPointF &scenePos ); 
		QPointF topLeftPage( int page );

		//void event( QEvent *e );
};
		

#endif /* _pdfScene_H */
