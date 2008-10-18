/***************************************************************
 * mainWindow.cpp
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
#include <QtGui/QLineEdit>
#include <QtGui/QAction>
#include <QtGui/QToolBar>
#include <QtGui/QGraphicsView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QMouseEvent>
#include <QtCore/QDebug>
#include <QtGui/QStackedWidget>

#include "mainWindow.h"
#include "testView.h"
#include "pageNumberEdit.h"
#include "toolBox.h"
#include "textTool.h"
#include "pdfScene.h"



mainWindow::mainWindow() { 
  toolBar = new toolBox( this );
  numberEdit = new pageNumberEdit( this );
  toolBar->addWidget( numberEdit );
  scene = new pdfScene();
  editor = new QStackedWidget( this );
  textAnnotTool = new textTool( scene, toolBar, editor );
  scene->registerTool( textAnnotTool );

  pageView = new testView( scene );
  pageView->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  pageView->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

  QAction *quitAct = new QAction( pageView );
  QAction *zoomInAct = new QAction( pageView );
  QAction *zoomOutAct = new QAction( pageView );

  quitAct->setShortcut((QString) "Ctrl+Q");
  zoomInAct->setShortcut((QString) "Ctrl++");
  zoomOutAct->setShortcut((QString) "Ctrl+-");

  pageView->addAction(zoomInAct);
  pageView->addAction(zoomOutAct);
  pageView->addAction(quitAct);

  connect( quitAct, SIGNAL( triggered() ), this, SIGNAL( quit() ) );
  connect( zoomInAct, SIGNAL( triggered() ), pageView, SLOT( zoomIN() ) );
  connect( zoomOutAct, SIGNAL( triggered() ), pageView, SLOT( zoomOUT() ) );
  connect( numberEdit, SIGNAL( prevPage() ), pageView, SLOT( prevPage() ) );
  connect( numberEdit, SIGNAL( nextPage() ), pageView, SLOT( nextPage() ) );
  connect( numberEdit, SIGNAL( gotoPage(int) ), pageView, SLOT( gotoPage(int) ) );


  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget( toolBar );
  mainLayout->addWidget( pageView );
  mainLayout->addWidget( editor );
  mainLayout->setSpacing( 0 );
  mainLayout->setContentsMargins( 0, 0, 0, 0 );
  setLayout( mainLayout );

}



void mainWindow::mouseNearBorder( const QPoint &pos ) { 
  QPoint localPos = mapFromGlobal( pos );
  if ( localPos.y() < toolBar->height()-10 ) toolBar->show();
  else toolBar->hide();
}


bool mainWindow::loadFile( QString fileName ) { 
  if ( scene->loadFromFile( fileName, numberEdit, SLOT( setPageNumber(int) )  ) ) { 
    numberEdit->setMaxPageNum( scene->getNumPages() );
    return true;
  }
  return false;

}


#include "mainWindow.moc"
