#ifndef _teXjob_H
#define _teXjob_H

/***************************************************************
 * teXjob.h
 * @Author:      Jonathan Verner (jonathan.verner@matfyz.cz)
 * @License:     GPL v2.0 or later
 * @Created:     2008-11-17.
 * @Last Change: 2008-11-17.
 * @Revision:    0.0
 * Description:
 * Usage:
 * TODO:
 *CHANGES:
 ***************************************************************/


#include <QtCore/QEventLoop>
#include <QtCore/QTemporaryFile>
#include <QtCore/QProcess>

#include <QtGui/QPixmap>


class compileJob : public QObject { 
  Q_OBJECT
	protected:
		static QString latexPath;
		static QString gsPath;
		static bool checkPaths( QString latex, QString gs );
		static bool paths_ok;

		static QString texName2Pdf( QString fname );

		QTemporaryFile *tmpSRC;
		QString pdfFName;
		void removeTempFiles();
		QRectF parseGSOutput();


		QProcess *proc;
		bool jobStarted;

	protected slots:

		void texJobFinished(int,QProcess::ExitStatus);
		void gsJobFinished(int,QProcess::ExitStatus);



	public:
		compileJob();
		~compileJob();

		static void setPaths( QString latex, QString gs );
		static bool pathsOK();

		void start( QString latexSource );
		void restart( QString latexSource );
		void kill();
		bool running();
	signals:
		void finished( QString resultPath, QRectF bBox, bool status );
};

namespace Poppler { 
  class Document;
}

class renderItem : public QObject { 
  Q_OBJECT
	private:
		QString pre,src;
		QString pdfFileName;
		QRectF bBox;
		Poppler::Document *pdf;
		int job_id;
		compileJob job;
		bool ready;
		
		
		QEventLoop *localLoop;
		bool waiting_for_job;
		void wait_for_job();

	private slots:
		void pdfReady( QString pdfFName, QRectF bBox, bool status );

	protected:
		static QString getLaTeX( QString source, QString preambule, int sizeHint );

	public:
		renderItem( QString source, QString preambule );
		~renderItem();
		void updateItem( QString source, QString preambule, int jobID = 0, bool format_inline = false, int sizeHint = 50 );
		int size();
		QPixmap render( qreal zoom, bool format_inline, int sizeHint );
		void preRender( int jobID, bool format_inline, int sizeHint );
		QString getPDFFileName() const { return pdfFileName; };
		QRectF getBBox() const { return bBox; };

	signals:
		void renderingReady( int jobID );
};


#endif /* _teXjob_H */
