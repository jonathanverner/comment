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
#include <QtCore/QPoint>
#include <QtCore/QString>
#include <poppler-qt4.h>


class QLineEdit;
class QAction;
class QToolBar;
class QGraphicsView;
class QGraphicsScene;
class QMouseEvent;
class testView;
class QRegExpValidator;




class mainWindow : public QWidget { 
  Q_OBJECT
	private:
		QLineEdit *pageNums;
		QRegExpValidator *val;
		QAction *nextAct, *prevAct;
		QToolBar *toolBar;
		testView *pageView;
		QGraphicsScene *scene;
		QWidget *editor;


		void createToolBar();
		void createActions();
		void createOther();
		void assemble();

		Poppler::Document *pdf;
		int num_of_pages;
	public:
		mainWindow();
		bool loadFile( QString fileName );

	protected slots:
		void mouseNearBorder(const QPoint &pos);
		void showPageNum( int pageNum );

	signals:
		void quit();

	protected:
};

#endif /* _mainWindow_H */
