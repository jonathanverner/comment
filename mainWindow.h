#ifndef _mainWindow_H
#define _mainWindow_H


/**  This file is part of comment
*
*  File: mainWindow.h
*  Created: 13. 10. 2008
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

#include <QtGui/QWidget>

#include <QtCore/QPoint>
#include <QtCore/QString>
#include <QtCore/QModelIndex>

#include <poppler-qt4.h>


class toolBox;
class pageView;
class pdfScene;
class textTool;
class pageNumberEdit;
class searchBar;
class searcher;

class QStackedWidget;
class QTreeView;
class QAction;


class mainWindow : public QWidget { 
  Q_OBJECT
	private:
	  	pageNumberEdit *numberEdit;
		toolBox *toolBar;
		pageView *pgView;
		QTreeView *tocView;
		pdfScene *scene;
		QStackedWidget *editor;
		textTool *textAnnotTool;
		searchBar *searchDlg;
		searcher *search;


		void createToolBar();
		void createActions();
		void createOther();
		void assemble();

	public:
		mainWindow();
		bool loadFile( QString fileName );

	public slots:
		void hideEditArea();
		void ensureVisible( const QRectF &rect );
		void showInfoDlg();
		void tocItemActivated( const QModelIndex &itemIndex );

	protected slots:
		void mouseNearBorder(const QPoint &pos);
		void pageNumEdit();
		void save();
		void showSearchBar();
		void toggleToc();
	signals:
		void quit();

	protected:
};

#endif /* _mainWindow_H */
