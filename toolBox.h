#ifndef _toolBox_H
#define _toolBox_H

/**  This file is part of comment
*
*  File: toolBox.h
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
