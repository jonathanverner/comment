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
#include "hilightTool.h"



mainWindow::mainWindow() { 
  scene = new pdfScene();
  pgView = new pageView( scene );
  toolBar = new toolBox( pgView );
  editor = new QStackedWidget( this );
  numberEdit = new pageNumberEdit( this );


  textAnnotTool = new textTool( scene, toolBar, editor );
  textAnnotTool->setAuthor( "Jonathan Verner" );
  toolBar->addWidget( numberEdit );
  hilightTool *hiTool = new hilightTool( scene, toolBar, editor );
  hiTool->setAuthor( "Jonathan Verner" );
  scene->registerTool( hiTool );
  scene->registerTool( textAnnotTool );
  toolBar->resize( pgView->width(), 50 );



  pgView->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  pgView->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

  QAction *quitAct = new QAction( pgView );
  QAction *saveAct = new QAction( pgView );
  QAction *zoomInAct = new QAction( pgView );
  QAction *zoomOutAct = new QAction( pgView );

  saveAct->setShortcut((QString) "Ctrl+S");
  quitAct->setShortcut((QString) "Ctrl+Q");
  zoomInAct->setShortcut((QString) "Ctrl++");
  zoomOutAct->setShortcut((QString) "Ctrl+-");

  pgView->addAction(zoomInAct);
  pgView->addAction(zoomOutAct);
  pgView->addAction(quitAct);
  pgView->addAction(saveAct);

  connect( quitAct, SIGNAL( triggered() ), this, SIGNAL( quit() ) );
  connect( saveAct, SIGNAL( triggered() ), this, SLOT( save() ) );
  connect( zoomInAct, SIGNAL( triggered() ), pgView, SLOT( zoomIN() ) );
  connect( zoomOutAct, SIGNAL( triggered() ), pgView, SLOT( zoomOUT() ) );
  connect( numberEdit, SIGNAL( prevPage() ), pgView, SLOT( prevPage() ) );
  connect( numberEdit, SIGNAL( nextPage() ), pgView, SLOT( nextPage() ) );
  connect( numberEdit, SIGNAL( gotoPage(int) ), pgView, SLOT( gotoPage(int) ) );
  connect( pgView, SIGNAL( mouseNearBorder(const QPoint&) ), this, SLOT( mouseNearBorder(const QPoint&) ) );
//  connect( pgView, SIGNAL( newAnnotationAction(const QPointF&) ), this, SLOT( newAnnotation(const QPointF &) ) );
  connect( toolBar, SIGNAL( toolActivated(abstractTool*) ), pgView, SLOT( setCurrentTool(abstractTool*) ) );


  QVBoxLayout *mainLayout = new QVBoxLayout;
 // mainLayout->addWidget( toolBar );
  mainLayout->addWidget( pgView );
  mainLayout->addWidget( editor );
  mainLayout->setSpacing( 0 );
  mainLayout->setContentsMargins( 0, 0, 0, 0 );
  setLayout( mainLayout );

  editor->hide();
}




void mainWindow::save() { 
  qDebug() << "mainWindow::save(): Saving...";
  if ( scene ) { 
    scene->save();
  } else { 
    qWarning() << "mainWindow::save(): pdfScene is a null pointer";
  }
}

void mainWindow::mouseNearBorder( const QPoint &pos ) { 
  QPoint localPos = mapFromGlobal( pos );
  if ( localPos.y() < toolBar->height()-10 ) {
    if ( ! toolBar->isVisible() ) toolBar->resize( pgView->width(), toolBar->height() );
    toolBar->show();
  }
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
