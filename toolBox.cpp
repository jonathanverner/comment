/**  This file is part of project comment
 *
 *  File: toolBox.cpp
 *  Created: 2008-10-18
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




#include "toolBox.h"
#include "abstractTool.h"
#include "myAction.h"

#include <QtCore/QDebug>

#include <QtGui/QAction>
#include <QtGui/QLineEdit>
#include <QtGui/QPainter>


myAction::myAction( const QIcon &icn, const QString text, QObject *parent, abstractTool *Tool ):
	QAction( icn, text, parent ), tool(Tool)
{
  if ( tool ) connect( this, SIGNAL( triggered() ), this, SLOT( activate() ) );
}

void myAction::activate() { 
  emit triggered( tool );
}

  /* If mainWin != NULL, then new actions will also be
   * added to mainWin, so that their shortcuts work when mainWin
   * has focus */
toolBox::toolBox( QWidget *mainWin ) : QToolBar( mainWin ), curTool( NULL ), actionWidget(mainWin), left(false), firstAction(NULL) {
  setAutoFillBackground( true );
  left=false;
}


void toolBox::setCurrentTool( abstractTool *tool ) {
  qDebug() << "Setting tool to " << tool->getToolName();
  curTool = tool;
//  emit toolChanged();
  emit toolActivated( tool );
}


void toolBox::addTool( const QIcon &icon, abstractTool *tool ) {
  if ( tools.contains( tool ) ) return;
  tools.insert( tool );

  myAction *toolAction = new myAction( icon, tool->getToolName(), actionWidget, tool );
  toolAction->setShortcut( QString("Ctrl+")+QString::number( tools.size() ) );
  if ( actionWidget ) actionWidget->addAction( toolAction );
  connect( toolAction, SIGNAL( triggered(abstractTool*) ), this, SLOT( setCurrentTool(abstractTool*) ) );
//  if ( left ) { // last tool was inserted on the left of the page number edit
                // so now we insert to the right
    addAction( toolAction );
/*    qDebug() << "Adding to the right";
    left = false;
  } else { // otherwise we insert to the left
    insertAction( firstAction, toolAction );
    firstAction = toolAction;
    qDebug() << "Adding to the left";
    left = true;
  }*/
}

/* FIXME: does not reorder the actions so that the difference between
 *        the number of actions to the left and right of the page 
 *        number edit is at most one.*/
void toolBox::removeTool( abstractTool *tool ) {
  myAction *myAct;
  foreach( QAction *act, actions() ) { 
    if ( myAct = dynamic_cast<myAction *>( act ) ) { 
      if ( myAct->isActionForTool( tool ) ) { 
	removeAction( act );
	tools.remove( tool );
	break;
      }
    }
  }

}

abstractTool *toolBox::currentTool() {
  return curTool;
}

void toolBox::leaveEvent( QEvent *e ) { 
  hide();
  QWidget::leaveEvent( e );
}


#include "toolBox.moc"
#include "myAction.moc"
