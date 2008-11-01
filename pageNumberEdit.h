#ifndef _pageNumberEdit_H
#define _pageNumberEdit_H

/***************************************************************
 * pageNumberEdit.h
 * @Author:      Jonathan Verner (jonathan.verner@matfyz.cz)
 * @License:     GPL v2.0 or later
 * @Created:     2008-10-18.
 * @Last Change: 2008-10-18.
 * @Revision:    0.0
 * Description:
 * Usage:
 * TODO:
 *CHANGES:
 ***************************************************************/
#include <QtCore/QString>
#include <QtGui/QToolBar>
class QLineEdit;

class pageNumberEdit : public QToolBar { 
  Q_OBJECT
	private:
	  	QWidget *actionWidget;
		/* page number edit */
		QLineEdit *pageEdit;
		int lastValidPageNumber, numberOfPages; 

  	protected slots:
		/* Taking care of the page number widget */
		void pageNumberChanged();
		void cursorChanged( int old, int nw );
		QString getPagePartOfEdit();

		/* next, previous handlers */
		void wantNext();
		void wantPrev();
		void wantGoTo();
	public:
		/* Beware, if you pass mainWin as
		 * NULL, the actions shortcut
		 * will only work when the toolBox
		 * has focus */
		pageNumberEdit( QWidget *main = NULL);

		void setMaxPageNum( int pageNum );

		// WARNING: as opposed to the signal gotoPage, this
		// function returns the pagenumber (where pagenumbers
		// start from 1)
		int getCurrentPageNum();


	public slots:
		void setPageNumber(int pgNum);




	signals:
		/* only emitted when the next/prev page is within the valid range 
		 * of pages, i.e. [1, MaxPageNum] */
		void nextPage();
		void prevPage();

		/* WARNING: pgNumber is zero-based */
		void gotoPage( int pgNumber );
};




#endif /* _pageNumberEdit_H */
