/***************************************************************
 * testPdfPageItem.cpp
 * @Author:      Jonathan Verner (jonathan.verner@matfyz.cz)
 * @License:     GPL v2.0 or later
 * @Created:     2008-10-12.
 * @Last Change: 2008-10-12.
 * @Revision:    0.0
 * Description:
 * Usage:
 * TODO:
 *CHANGES:
 ***************************************************************/


#include <poppler-qt4.h>
#include <QtGui/QApplication>
#include <QtCore/QString>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtGui/QAction>
#include <QtGui/QImage>
#include <QtGui/QGraphicsScene>
#include <QtGui/QPixmap>
#include <QtGui/QLabel>

#include "testView.h"
#include "pdfPageItem.h"
#include "commentItem.h"

int main(int argc, char **argv) { 
  QApplication app( argc, argv );

  if ( argc < 3 ) {
    qDebug() << "Usage: " << argv[0] << " pdf-file page-number";
    return -1;
  }
    
  QString fileName( argv[1] ), pgn( argv[2] );
  bool ok;
  int pgNum = pgn.toInt(&ok);
  if ( ! ok ) { 
    qDebug() << "Error: " << pgn << "is not a number"; 
    qDebug() << "Usage: " << argv[0] << " pdf-file page-number";
    return -1;
  }

  if ( pgNum < 1 ) { 
    qDebug() << "Error: " << pgn << "is not a valid page number"; 
    qDebug() << "Usage: " << argv[0] << " pdf-file page-number";
    return -1;
  }

  pgNum--;

  QFile fl( fileName );
  if ( ! fl.exists() ) { 
    qDebug() << "File not found: " << fileName;
    return -1;
  }


  Poppler::Document *pdf = Poppler::Document::load( fileName );

  if ( ! pdf ) { 
    qDebug() << "Error reading file " << fileName;
    return -1;
  } else  {
    pdf->setRenderHint( Poppler::Document::TextAntialiasing, true );
    pdf->setRenderHint( Poppler::Document::Antialiasing, true );
  }


  if ( pdf->numPages() < pgNum ) { 
    qDebug() << pgNum <<": No such page in pdf file. Pdf file has " << pdf->numPages() << " pages.";
    return -1;
  }

  Poppler::Page *page = pdf->page( pgNum );
  QGraphicsScene scene;
//  pdfPageItem pdfPage( page );
  commentItem comment(100,100);
  testView view( &scene );
  QPixmap comment_icon, hover;
  comment_icon.load("comment.png");
  hover.load("hover.png");
  pdfPageItem *pageItem;
  qreal y=0;

  for(int i = 0; i < pdf->numPages(); i++ ) {
    pageItem = new pdfPageItem( pdf->page( i ) );
    pageItem->setPos(10,y);
    y+=pageItem->boundingRect().height()+10;
    scene.addItem( pageItem );
  };

//  scene.addItem( &pdfPage );
  scene.addItem( &comment );
  scene.setBackgroundBrush(Qt::gray);
  comment.setZValue( 1 );
  comment.setIcon( comment_icon );
  comment.setText(" Toto je testovaci comment " );
//  view.ignore_item_interaction( &pdfPage );
//  comment.setToolTip( hover );
//  scene.setSceneRect(0,0,600,800);
//  testPageView view( &scene, page, 0 );

//  scene.addRect(QRect(0,0,50,50));
//  view.ukaz();
  view.show();
  


  QAction *quitAct = new QAction( &view );
  /*QAction *nextAct = new QAction( &view );*/
  /*QAction *prevAct = new QAction( &view );*/

  QAction *zoomInAct = new QAction( &view );
  QAction *zoomOutAct = new QAction( &view );

  /*QAction *leftAct = new QAction( &view );*/
  /*QAction *rightAct = new QAction( &view );*/
  /*QAction *upAct = new QAction( &view );*/
  /*QAction *downAct = new QAction( &view );*/

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


  view.addAction(quitAct);
  /*view.addAction(nextAct);*/
  /*view.addAction(prevAct);*/

  view.addAction(zoomInAct);
  view.addAction(zoomOutAct);

  /*view.addAction(leftAct);*/
  /*view.addAction(rightAct);*/
  /*view.addAction(upAct);*/
  /*view.addAction(downAct);*/


  QObject::connect( quitAct, SIGNAL( triggered() ), &app, SLOT( quit() ) );
  /*QObject::connect( nextAct, SIGNAL( triggered() ), &view, SLOT( nextview() ) );*/
  /*QObject::connect( prevAct, SIGNAL( triggered() ), &view, SLOT( prevview() ) );*/

  QObject::connect( zoomInAct, SIGNAL( triggered() ), &view, SLOT( zoomIN() ) );
  QObject::connect( zoomOutAct, SIGNAL( triggered() ), &view, SLOT( zoomOUT() ) );

/*  QObject::connect( leftAct, SIGNAL( triggered() ), &view, SLOT( left() ) );
  QObject::connect( rightAct, SIGNAL( triggered() ), &view, SLOT( right() ) );
  QObject::connect( upAct, SIGNAL( triggered() ), &view, SLOT( up() ) );
  QObject::connect( downAct, SIGNAL( triggered() ), &view, SLOT( down() ) ); */

  app.exec();

  return 0;
}


 
