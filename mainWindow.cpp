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



  toolBar->addWidget( numberEdit );

  textAnnotTool = new textTool( scene, toolBar, editor );
  textAnnotTool->setAuthor( "Jonathan Verner" );
  hilightTool *hiTool = new hilightTool( scene, toolBar, editor );
  hiTool->setAuthor( "Jonathan Verner" );
  scene->registerTool( hiTool );
  scene->registerTool( textAnnotTool );
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
