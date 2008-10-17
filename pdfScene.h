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
#include <QtCore/QList>

class abstractTool;
class PoDoFo::PdfDocument;
class Poppler::Document;

class pdfScene : public QGraphicsScene { 
	private:
		QSet<abstractTool *> tools;
		QVector<QPoint> pageCorners;
		qreal pageSkip; // amount of space to be left between the pages of the pdf
		qreal leftSkip; // the left margin
		QString myFileName;
		QByteArray tempFileName;
		int numPages;

		Poppler::Document *pdf;

		void processPage( PoDoFo::PdfDocument *pdf, int pgNum );
		void loadPopplerPdf( QString fileName );
		void mergeAnnotationsFromPage( PoDoFo::PdfDocument *pdf, int pgNum );
		void generateTempFileName();
		void updateAnnotationPositions();

	public:
		pdfScene();
		pdfScene( const QList<abstractTool *> &tools, QString fileName );

		void registerTool( abstractTool *tool );

		bool loadFromFile( QString fileName );
		bool saveToFile( QString fileName );
		bool save();

		/* Always retuns a valid page number in [0,numPages) */
		int posToPage( const QPoint &scenePos ); 
		QPoint topLeftPage( int page );
};
		

#endif /* _pdfScene_H */
