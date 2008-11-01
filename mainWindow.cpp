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
#include "pageView.h"



mainWindow::mainWindow() { 
  toolBar = new toolBox( this );
  numberEdit = new pageNumberEdit( this );
  toolBar->addWidget( numberEdit );
  scene = new pdfScene();
  editor = new QStackedWidget( this );
  textAnnotTool = new textTool( scene, toolBar, editor );
  scene->registerTool( textAnnotTool );



  pgView = new pageView( scene );
  pgView->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  pgView->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

  QAction *quitAct = new QAction( pgView );
  QAction *zoomInAct = new QAction( pgView );
  QAction *zoomOutAct = new QAction( pgView );

  quitAct->setShortcut((QString) "Ctrl+Q");
  zoomInAct->setShortcut((QString) "Ctrl++");
  zoomOutAct->setShortcut((QString) "Ctrl+-");

  pgView->addAction(zoomInAct);
  pgView->addAction(zoomOutAct);
  pgView->addAction(quitAct);

  connect( quitAct, SIGNAL( triggered() ), this, SIGNAL( quit() ) );
  connect( zoomInAct, SIGNAL( triggered() ), pgView, SLOT( zoomIN() ) );
  connect( zoomOutAct, SIGNAL( triggered() ), pgView, SLOT( zoomOUT() ) );
  connect( numberEdit, SIGNAL( prevPage() ), pgView, SLOT( prevPage() ) );
  connect( numberEdit, SIGNAL( nextPage() ), pgView, SLOT( nextPage() ) );
  connect( numberEdit, SIGNAL( gotoPage(int) ), pgView, SLOT( gotoPage(int) ) );
  connect( pgView, SIGNAL( mouseNearBorder(const QPoint&) ), this, SLOT( mouseNearBorder(const QPoint&) ) );
//  connect( pgView, SIGNAL( newAnnotationAction(const QPointF&) ), this, SLOT( newAnnotation(const QPointF &) ) );
  connect( toolBar, SIGNAL( toolActivated(abstractTool*) ), pgView, SLOT( setCurrentTool(abstractTool*) ) );


  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget( toolBar );
  mainLayout->addWidget( pgView );
  mainLayout->addWidget( editor );
  mainLayout->setSpacing( 0 );
  mainLayout->setContentsMargins( 0, 0, 0, 0 );
  setLayout( mainLayout );
  editor->hide();
}


void mainWindow::newAnnotation( const QPointF &scenePos ) { 
 abstractTool *tool = toolBar->currentTool();
 if ( tool ) {
 qDebug() << "Main Window New Annotation";
   tool->newActionEvent( &scenePos );
 }
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
