#ifndef _myAction_H
#define _myAction_H

/**  This file is part of comment
*
*  File: myAction.h
*  Created: 18. 10. 2009
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

#include <QtGui/QAction>

class abstractTool;

class myAction : public QAction  {
  Q_OBJECT
	private:
		abstractTool *tool;		
	private slots:
		void activate();
	public:
		myAction( const QIcon &icn, const QString text, QObject *parent, abstractTool *tool );
		bool isActionForTool( abstractTool *Tool ) { return ( tool == Tool ); };
	signals:
		void triggered( abstractTool *tool );
};



#endif /* _myAction_H */
