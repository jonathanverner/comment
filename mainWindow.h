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


class toolBox;
class testView;
class pdfScene;
class textTool;
class pageNumberEdit;

class QStackedWidget;

class mainWindow : public QWidget { 
  Q_OBJECT
	private:
	  	pageNumberEdit *numberEdit;
		toolBox *toolBar;
		testView *pageView;
		pdfScene *scene;
		QStackedWidget *editor;
		textTool *textAnnotTool;


		void createToolBar();
		void createActions();
		void createOther();
		void assemble();

	public:
		mainWindow();
		bool loadFile( QString fileName );

	protected slots:
		void mouseNearBorder(const QPoint &pos);
		void newAnnotation( const QPointF &scenePos);

//		void updatePageNumber( int pgNum );

	signals:
		void quit();

	protected:
};

#endif /* _mainWindow_H */
