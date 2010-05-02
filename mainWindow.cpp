/**  This file is part of project comment
 *
 *  File: mainWindow.cpp
 *  Created: 2008-10-13
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



#include "mainWindow.h"
#include "testView.h"
#include "pageNumberEdit.h"
#include "toolBox.h"
#include "textTool.h"
#include "hilightTool.h"
#include "inlineTextTool.h"
#include "pdfScene.h"
#include "pageView.h"
#include "searchBar.h"
#include "search.h"
#include "config.h"

#include "pdfUtil.h"
#include "ui_propertyDialog.h"

#include <QtGui/QLineEdit>
#include <QtGui/QAction>
#include <QtGui/QToolBar>
#include <QtGui/QGraphicsView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QMouseEvent>
#include <QtCore/QDebug>
#include <QtGui/QStackedWidget>

mainWindow::mainWindow() { 
  scene = new pdfScene();
  pgView = new pageView( scene );
  toolBar = new toolBox( pgView );
  editor = new QStackedWidget( this );
  numberEdit = new pageNumberEdit( this );
  searchDlg = new searchBar( this );
  search = new searcher( scene );



  toolBar->addWidget( numberEdit );
  editor->addWidget( searchDlg );

  textAnnotTool = new textTool( scene, toolBar, editor );
  textAnnotTool->setAuthor( config()["author"] );
  inlineTextTool *itTool = new inlineTextTool( scene, toolBar, editor );
  itTool->setAuthor( config()["author"] );
  hilightTool *hiTool = new hilightTool( scene, toolBar, editor ); 
  hiTool->setAuthor( config()["author"] );
  scene->registerTool( hiTool );
  scene->registerTool( textAnnotTool );
  scene->registerTool( itTool );
  toolBar->resize( pgView->width(), toolBar->height()+10 );



  pgView->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  pgView->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

  QAction *quitAct = pgView->newAction( "Ctrl+Q", this, SIGNAL( quit() ) );
  QAction *saveAct = pgView->newAction( "Ctrl+S", this, SLOT( save() ) );
  QAction *zoomInAct = pgView->newAction( "Ctrl++", pgView, SLOT( zoomIN() ) );
  QAction *zoomOutAct = pgView->newAction( "Ctrl+-", pgView, SLOT( zoomOUT() ) );
  QAction *pageGotoAct = pgView->newAction( "F6", this, SLOT( pageNumEdit() ) );
  QAction *nextPageAct = pgView->newAction( "PgDown", pgView, SLOT( nextPage() ) );
  QAction *prevPageAct = pgView->newAction( "PgUp", pgView, SLOT( prevPage() ) );
  QAction *upAct = pgView->newAction( "Up", pgView, SLOT( up() ) );
  QAction *downAct = pgView->newAction( "Down", pgView, SLOT( down() ) );
  QAction *rightAct = pgView->newAction( "Right", pgView, SLOT( right() ) );
  QAction *leftAct = pgView->newAction( "Left", pgView, SLOT( left() ) );
  QAction *startAct =  pgView->newAction( "Ctrl+Home", pgView, SLOT( firstPage() ) );
  QAction *endAct = pgView->newAction( "Ctrl+End", pgView, SLOT( lastPage() ) );
  QAction *searchAct = pgView->newAction( "Ctrl+F", this, SLOT( showSearchBar() ) );
  QAction *infoAct = pgView->newAction( "Ctrl+I", this, SLOT( showInfoDlg() ) );


  connect( itTool, SIGNAL( needKeyFocus(bool) ), pgView, SLOT( disableActions(bool) ) );
  connect( numberEdit, SIGNAL( prevPage() ), pgView, SLOT( prevPage() ) );
  connect( numberEdit, SIGNAL( nextPage() ), pgView, SLOT( nextPage() ) );
  connect( numberEdit, SIGNAL( gotoPage(int) ), pgView, SLOT( gotoPage(int) ) );
  connect( pgView, SIGNAL( mouseNearBorder(const QPoint&) ), this, SLOT( mouseNearBorder(const QPoint&) ) );
//  connect( pgView, SIGNAL( newAnnotationAction(const QPointF&) ), this, SLOT( newAnnotation(const QPointF &) ) );
  connect( toolBar, SIGNAL( toolActivated(abstractTool*) ), pgView, SLOT( setCurrentTool(abstractTool*) ) );


  connect( searchDlg, SIGNAL( textChanged(QString) ), search, SLOT( searchTermChanged(QString) ) );
  connect( searchDlg, SIGNAL( nextMatch() ), search, SLOT( nextMatch() ) ); 
  connect( searchDlg, SIGNAL( prevMatch() ), search, SLOT( prevMatch() ) );

  connect( search, SIGNAL( matchFound(int) ), searchDlg, SLOT( setFound() ) );
  connect( search, SIGNAL( matchNotFound() ), searchDlg, SLOT( setMissed() ) );
  connect( search, SIGNAL( clear() ), searchDlg, SLOT( setNone() ) );
  connect( search, SIGNAL( currentMatchPosition(const QRectF&) ), this, SLOT( ensureVisible(const QRectF&) ) );



  QVBoxLayout *mainLayout = new QVBoxLayout;
 // mainLayout->addWidget( toolBar );
  mainLayout->addWidget( pgView );
  mainLayout->addWidget( editor );
  mainLayout->setSpacing( 0 );
  mainLayout->setContentsMargins( 0, 0, 0, 0 );
  setLayout( mainLayout );

  editor->hide();
}

void mainWindow::hideEditArea() { 
  editor->hide();
  pgView->setFocus();
}

void mainWindow::ensureVisible( const QRectF &rect ) { 
  pgView->ensureVisible( rect );
}

void mainWindow::showSearchBar() { 
  search->clearSearch();
  editor->setCurrentWidget( searchDlg );
  editor->show();
  searchDlg->focus();
}


void mainWindow::save() { 
  qDebug() << "mainWindow::save(): Saving...";
  if ( scene ) { 
    scene->save();
  } else { 
    qWarning() << "mainWindow::save(): pdfScene is a null pointer";
  }
}

void mainWindow::showInfoDlg() { 
  pdfProperties prop;
  QDialog *dlg = new QDialog( (QWidget *) this );
  Ui::Dialog propDlg;
  propDlg.setupUi( dlg );
  scene->getPdfProperties( prop );
  propDlg.titleEdit->setText( prop.title );
  propDlg.authorEdit->setText( prop.author );
  propDlg.subjectEdit->setText( prop.subject );
  propDlg.keywordsEdit->setText( prop.keywords );
  dlg->show();
  int result=dlg->exec();
  if ( result == 0 ) return;
  prop.title = propDlg.titleEdit->text();
  prop.author = propDlg.authorEdit->text();
  prop.subject = propDlg.subjectEdit->text();
  prop.keywords = propDlg.keywordsEdit->toPlainText();
  scene->setPdfProperties( prop );
  delete dlg;
}

  

  

void mainWindow::pageNumEdit() { 
    if ( ! toolBar->isVisible() ) toolBar->resize( pgView->width(), toolBar->height() );
    toolBar->show();
    numberEdit->setFocus();
    numberEdit->selectNumber();
}
 

void mainWindow::mouseNearBorder( const QPoint &pos ) { 
  QPoint localPos = mapFromGlobal( pos );
  if ( localPos.y() < toolBar->height()-10 ) {
    if ( ! toolBar->isVisible() ) toolBar->resize( pgView->width(), toolBar->height() );
    toolBar->show();
  }
  else { 
    toolBar->hide();
    pgView->setFocus();
  }
}


bool mainWindow::loadFile( QString fileName ) { 
  if ( scene->loadFromFile( fileName, numberEdit, SLOT( setPageNumber(int) )  ) ) { 
    numberEdit->setMaxPageNum( scene->getNumPages() );
    if ( config().haveKey( fileName ) ) { 
      qDebug() << "Goto page" << config()[fileName];
      pgView->gotoPage( config()[fileName].toInt() );
    }
    return true;
  }
  return false;

}


#include "mainWindow.moc"
