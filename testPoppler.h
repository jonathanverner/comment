#ifndef _testPoppler_H
#define _testPoppler_H

/***************************************************************
 * testPoppler.h
 * @Author:      Jonathan Verner (jonathan.verner@matfyz.cz)
 * @License:     GPL v2.0 or later
 * @Created:     2008-10-10.
 * @Last Change: 2008-10-10.
 * @Revision:    0.0
 * Description:
 * Usage:
 * TODO:
 *CHANGES:
 ***************************************************************/
#include <QtGui/QLabel>
#include <QtCore/QString>

class Poppler::Document;

class pdfBoard : public QLabel { 
  Q_OBJECT
	private:
		Poppler::Document *pdf;
		int currentPage, maxPages,x,y;
		double zoom;
		void updateBoard();
		void init();
	public:
		pdfBoard( QString fileName , int page );
		pdfBoard( QString fileName );
		~pdfBoard();

	public slots:
		void up();
		void down();
		void left();
		void right();
		void zoomIN();
		void zoomOUT();
		void nextPage();
		void prevPage();
		void goTO(int page);
};



#endif /* _testPoppler_H */
