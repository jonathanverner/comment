/**  This file is part of project comment
 *
 *  File: pageNumberEdit.cpp
 *  Created: 02.05.2010
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


#include "pageNumberEdit.h"
#include "config.h"

#include <QtCore/QDebug>
#include <QtCore/QRegExp>

#include <QtGui/QLineEdit>
#include <QtGui/QAction>


pageNumberEdit::pageNumberEdit( QWidget *main ):
	QToolBar( main ), actionWidget( NULL )
{ 

  /* Create the Next, Prev actions and the line edit
   * and add them to the toolbar */
  QAction *prevAct = addAction( QIcon::fromTheme("go-previous"), tr("&Previous Page"), this, SLOT( wantPrev() ) );
  pageEdit = new QLineEdit( this );
  addWidget( pageEdit );
  QAction *nextAct = addAction( QIcon::fromTheme("go-next"), tr("&Next Page"), this, SLOT( wantNext() ) );

 
  nextAct->setShortcut( QString( "Ctrl+N" ) );
  prevAct->setShortcut( QString( "Ctrl+P" ) );

  /* Line Edit setup */
  pageEdit->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  lastValidPageNumber = 0;
  setMaxPageNum( 0 );
  pageEdit->setCursorPosition(0);
  connect( pageEdit, SIGNAL( textEdited(QString) ), this, SLOT( pageNumberChanged() ) );
  connect( pageEdit, SIGNAL( cursorPositionChanged(int,int) ), this, SLOT( cursorChanged(int,int) ) ); 
  connect( pageEdit, SIGNAL( editingFinished() ), this, SLOT( wantGoTo() ) );

  /* If mainWin is a widget, add the actions there, so that they are
   * global and shortcuts work globaly */
  if ( actionWidget = main ) { 
    actionWidget->addAction( nextAct );
    actionWidget->addAction( prevAct );
  }
  setMaximumSize(200,300);
}

/* Guarantees that the cursor stays within the editable part
 * of the widget, i.e. before the slash (we can only edit
 * the current page, not the number of pages ) */
void pageNumberEdit::cursorChanged( int old, int nw ) {
  QString pagePart = getPagePartOfEdit();
  if ( nw > pagePart.size() + 1 ) pageEdit->setCursorPosition( old );
};

/* called whenever the page number edit is changed
 * if the change is not valid, then it reverts the
 * change to the last valid pageNumber and sets
 * the cursor position to 0 before returning
 */

void pageNumberEdit::pageNumberChanged() { 
  QString pagePart = getPagePartOfEdit();
  if ( pagePart == "" ) return;
  int cPos = pageEdit->cursorPosition();
  bool ok;
  int newPageNumber = pagePart.toInt(&ok);
  if ( ok && 1 <= newPageNumber && newPageNumber <= numberOfPages ) { 
    return;
  } else { 
    setPageNumber( lastValidPageNumber );
    pageEdit->setCursorPosition( 0 );
  }
}

void pageNumberEdit::setPageNumber( int pgNum ) {
  if ( pgNum < 1 || pgNum > numberOfPages ) pgNum = lastValidPageNumber;
  else lastValidPageNumber = pgNum;
  pageEdit->setText(QString::number( pgNum ) + QString(" / ") + QString::number( numberOfPages ) );
  pageEdit->setCursorPosition(0);
  config()["current_page"]=QString::number(pgNum);
}
		  
/* if the page edit text is of the form 
 * [1-9]+[0-9]* / [1-9]+[0-9]*
 * (i.e. a positive number / positive number )
 * then it returns the string representation
 * of the first number, otherwise it returns
 * the string "error" */
QString pageNumberEdit::getPagePartOfEdit() { 
  QRegExp exp( " *([1-9]*[0-9]*) */ *[1-9]+[0-9]*" );
  if ( ! exp.exactMatch( pageEdit->text() ) ) return "error";
  return exp.cap(1);
}

int pageNumberEdit::getCurrentPageNum() { 
  bool ok;
  int ret = getPagePartOfEdit().toInt( &ok );
  if (! ok ) return lastValidPageNumber;
  else return ret;
}

void pageNumberEdit::setMaxPageNum(int max) { 
  int curNum = getCurrentPageNum();
  numberOfPages = max;
  if ( curNum > max ) { 
    setPageNumber( max );
  } else if ( curNum < 1 ) { 
    setPageNumber( 1 );
  }
  pageEdit->setCursorPosition( 0 );
}

void pageNumberEdit::wantGoTo() { 
  int cur = getCurrentPageNum();
  if ( cur <= numberOfPages && 0 < cur) { 
    emit gotoPage( cur );
  }
}
    

void pageNumberEdit::wantNext() { 
  int cur = getCurrentPageNum();
  if ( cur < numberOfPages ) {
    setPageNumber( cur+1 );
    emit nextPage();
  }
}

void pageNumberEdit::wantPrev() { 
  int cur = getCurrentPageNum();
  if ( getCurrentPageNum() > 1 ) {
       setPageNumber( cur-1 );
       emit prevPage();
  }
}

void pageNumberEdit::selectNumber() { 
  pageEdit->setSelection(0,getPagePartOfEdit().size());
}

void pageNumberEdit::setFocus() {
  pageEdit->setFocus();
}

#include "pageNumberEdit.moc"
