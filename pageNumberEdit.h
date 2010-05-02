#ifndef _pageNumberEdit_H
#define _pageNumberEdit_H


/**  This file is part of comment
*
*  File: pageNumberEdit.h
*  Created: 18. 10. 2008
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
		void selectNumber();
		void setFocus();




	signals:
		/* only emitted when the next/prev page is within the valid range 
		 * of pages, i.e. [1, MaxPageNum] */
		void nextPage();
		void prevPage();

		/* WARNING: pgNumber is 1-based */
		void gotoPage( int pgNumber );
};




#endif /* _pageNumberEdit_H */
