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
#include <QtGui/QRegExpValidator>
#include <QtGui/QAction>
#include <QtGui/QToolBar>
#include <QtGui/QGraphicsView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QMouseEvent>
#include <QtCore/QDebug>

#include "mainWindow.h"
#include "pdfPageItem.h"
#include "pageBeginItem.h"
#include "testView.h"


void mainWindow::mouseNearBorder( const QPoint &pos ) { 
  QPoint localPos = mapFromGlobal( pos );
  if ( localPos.y() < toolBar->height()-10 ) toolBar->show();
  else toolBar->hide();
}

void mainWindow::createActions() {
  nextAct = new QAction(QIcon(":next.png"), tr("&Next Page"), this);
  prevAct = new QAction(QIcon(":prev.png"), tr("&Previous Page"), this);
  prevAct->setShortcut((QString) "Ctrl+P");
  nextAct->setShortcut((QString) "Ctrl+N");


  pageNums = new QLineEdit( this );
  pageNums->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  val = new QRegExpValidator( this );

  QAction *quitAct = new QAction( pageView );
  /*QAction *nextAct = new QAction( pageView );*/
  /*QAction *prevAct = new QAction( pageView );*/

  QAction *zoomInAct = new QAction( pageView );
  QAction *zoomOutAct = new QAction( pageView );

  /*QAction *leftAct = new QAction( pageView );*/
  /*QAction *rightAct = new QAction( pageView );*/
  /*QAction *upAct = new QAction( pageView );*/
  /*QAction *downAct = new QAction( pageView );*/

  quitAct->setShortcut((QString) "Ctrl+Q");
  /*nextAct->setShortcut((QString) "Ctrl+N");*/
  /*prevAct->setShortcut((QString) "Ctrl+P");*/

  zoomInAct->setShortcut((QString) "Ctrl++");
  zoomOutAct->setShortcut((QString) "Ctrl+-");
 /* */
  /*leftAct->setShortcut((QString) "Left");*/
  /*rightAct->setShortcut((QString) "Right");*/
  /*upAct->setShortcut((QString) "Up");*/
  /*downAct->setShortcut((QString) "Down");*/


  pageView->addAction(quitAct);
  /*pageView->addAction(nextAct);*/
  /*pageView->addAction(prevAct);*/

  pageView->addAction(zoomInAct);
  pageView->addAction(zoomOutAct);
  pageView->addAction(prevAct);
  pageView->addAction(nextAct);

  /*pageView->addAction(leftAct);*/
  /*pageView->addAction(rightAct);*/
  /*pageView->addAction(upAct);*/
  /*pageView->addAction(downAct);*/


  QObject::connect( quitAct, SIGNAL( triggered() ), this, SIGNAL( quit() ) );
  /*QObject::connect( nextAct, SIGNAL( triggered() ), pageView, SLOT( nextpageView() ) );*/
  /*QObject::connect( prevAct, SIGNAL( triggered() ), pageView, SLOT( prevpageView() ) );*/

  QObject::connect( zoomInAct, SIGNAL( triggered() ), pageView, SLOT( zoomIN() ) );
  QObject::connect( zoomOutAct, SIGNAL( triggered() ), pageView, SLOT( zoomOUT() ) );
  QObject::connect( prevAct, SIGNAL( triggered() ), pageView, SLOT( prevPage() ) );
  QObject::connect( nextAct, SIGNAL( triggered() ), pageView, SLOT( nextPage() ) );


/*  QObject::connect( leftAct, SIGNAL( triggered() ), pageView, SLOT( left() ) );
  QObject::connect( rightAct, SIGNAL( triggered() ), pageView, SLOT( right() ) );
  QObject::connect( upAct, SIGNAL( triggered() ), pageView, SLOT( up() ) );
  QObject::connect( downAct, SIGNAL( triggered() ), pageView, SLOT( down() ) ); */
}

void mainWindow::createToolBar() { 
  toolBar = new QToolBar( this );
  toolBar->addAction( prevAct );
  toolBar->addWidget( pageNums );
  toolBar->addAction( nextAct );
}
void mainWindow::createOther() {
  pageView = new testView( NULL );
  pageView->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  pageView->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  editor = new QWidget( this );
}

void mainWindow::assemble() {
  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget( toolBar );
  mainLayout->addWidget( pageView );
  mainLayout->addWidget( editor );
  mainLayout->setSpacing( 0 );
  mainLayout->setContentsMargins( 0, 0, 0, 0 );
  setLayout( mainLayout );
}

bool mainWindow::loadFile( QString fileName ) { 
  Poppler::Document *newPdf = Poppler::Document::load( fileName );
  if ( ! newPdf ) return false;
  if ( pdf ) { 
    pageView->setScene(0);
    delete scene;
  }
  num_of_pages = newPdf->numPages();
  scene = new QGraphicsScene(this);
  scene->setBackgroundBrush(Qt::gray);
  pdf = newPdf;
  pdf->setRenderHint( Poppler::Document::TextAntialiasing, true );
  pdf->setRenderHint( Poppler::Document::Antialiasing, true );
  pdfPageItem *pageItem;
  pageBeginItem *beginMarker;
  qreal y=0;
  for(int i = 0; i < num_of_pages; i++ ) {
    pageItem = new pdfPageItem( pdf->page( i ) );
    pageItem->setPos(10,y);
    QRectF pgSize = pageItem->boundingRect();
    beginMarker = new pageBeginItem( i+1, pgSize.width());
    beginMarker->setPos(10, y+pgSize.height()/2);
    y+=pageItem->boundingRect().height()+10;
    scene->addItem( pageItem );
    scene->addItem( beginMarker );
    connect( beginMarker->getSignalEmitter(), SIGNAL( pageInView(int) ), pageView, SIGNAL( onPage(int) ) );
  }
  val->setRegExp( QRegExp(QString("[0-9]* / ")+QString::number(num_of_pages)) );
  pageNums->setValidator( val );
  pageView->setScene( scene );
  return true;
}



mainWindow::mainWindow(): pdf(NULL), scene(NULL), editor(NULL) {
  createOther();
  createActions();
  createToolBar();
  assemble();
  setMouseTracking( true );
  setEnabled( true );
  connect( pageView, SIGNAL( mouseNearBorder( const QPoint & ) ), this, SLOT( mouseNearBorder( const QPoint & ) ) );
  connect( pageView, SIGNAL( onPage(int) ), this, SLOT( showPageNum(int) ) );
}

void mainWindow::showPageNum( int num ) { 
  pageNums->setText( QString::number( num ) + " / " + QString::number( num_of_pages ) );
  qDebug() << "Page number: "<< num;
}


#include "mainWindow.moc"
