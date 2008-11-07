/***************************************************************
 * toolBox.cpp
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

#include <QtCore/QDebug>

#include <QtGui/QAction>
#include <QtGui/QLineEdit>

#include "toolBox.h"
#include "abstractTool.h"
#include "myAction.h"


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
toolBox::toolBox( QWidget *mainWin ) : QToolBar( mainWin ), curTool( NULL ), actionWidget(mainWin) {
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
  if ( left ) { // last tool was inserted on the left of the page number edit
                // so now we insert to the right
    addAction( toolAction );
    left = false;
  } else { // otherwise we insert to the left
    insertAction( firstAction, toolAction );
    firstAction = toolAction;
    left = true;
  }
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
