#ifndef _mainWindow_H
#define _mainWindow_H

/***************************************************************
 * mainWindow.h
 * @Author:      Jonathan Verner (jonathan.verner@matfyz.cz)
 * @License:     GPL v2.0 or later
 * @Created:     2008-10-13.
 * @Last Change: 2008-10-13.
 * @Revision:    0.0
 * Description:
 * Usage:
 * TODO:
 *CHANGES:
 ***************************************************************/
#include <QtGui/QWidget>

class QLineEdit;
class QAction;
class QToolBar;
class QGraphicsView;
class QGraphicsScene;
class Poppler::Document;


class mainWindow : public QWidget { 
	private:
		QLineEdit *pageNums;
		QAction *nextAct, *prevAct;
		QToolBar *toolBar;
		QGraphicsView *pageView;
		QGraphicsScene *scene;
		QWidget *editor;

		void createToolBar();
		void createActions();
		void createOther();
		void assemble();

		Poppler::Document *pdf;
	public:
		mainWindow(Poppler::Document *pdf);
	
};

#endif /* _mainWindow_H */
