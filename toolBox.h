#ifndef _toolBox_H
#define _toolBox_H

/***************************************************************
 * toolBox.h
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

#include <QtCore/QSet>
#include <QtCore/QString>

#include <QtGui/QIcon>
#include <QtGui/QToolBar>

class abstractTool;
class QLineEdit;
class QEvent;

class toolBox : public QToolBar { 
  Q_OBJECT
	private:
		/* available tools */
		QSet<abstractTool *> tools;

		/* currently active tool */
		abstractTool *curTool;

		/* the widget, where actions
		 * are added */

		QWidget *actionWidget;

		/* was the last tool added to
		 * the left of the
		 * page number edit */
		bool left;

		QAction *firstAction;

		
	public:
		/* Beware, if you pass mainWin as
		 * NULL, the actions shortcut
		 * will only work when the toolBox
		 * has focus */
		toolBox( QWidget *mainWin );

		void addTool( const QIcon &icon, abstractTool *tool );
		void removeTool( abstractTool *tool );

		abstractTool *currentTool();

	protected:
		void leaveEvent( QEvent *event );

	public slots:
		void setCurrentTool( abstractTool *tool );


	signals:
		//void toolChanged();
		void toolActivated( abstractTool *tool );
};






#endif /* _toolBox_H */
