#include <poppler-qt4.h>
#include <QtGui/QApplication>
#include <QtCore/QString>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtGui/QAction>
#include <QtGui/QImage>


#include "testPoppler.h"

void pdfBoard::init() { 
  pdf->setRenderHint( Poppler::Document::TextAntialiasing, true );
  pdf->setRenderHint( Poppler::Document::Antialiasing, true );
//  pdf->setRenderBackend( Poppler::Document::SplashBackend );
  maxPages = pdf->numPages();
}

pdfBoard::pdfBoard( QString fileName, int page ): currentPage( page ),x(0),y(0),zoom(1) { 
  pdf = Poppler::Document::load( fileName );
  init();
  updateBoard();
}

pdfBoard::pdfBoard( QString fileName ): currentPage( 0 ), x(0), y(0), zoom(1) { 
  pdf = Poppler::Document::load( fileName );
  init();
  updateBoard();
}
pdfBoard::~pdfBoard() { 
  delete pdf;
}

void pdfBoard::zoomIN() {
  qDebug() << "ZoomIN" << zoom;
  if ( zoom >= 1 ) zoom ++;
  else if ( zoom > 0 ) zoom *= 2;
  else zoom = 0.1;
  updateBoard();
}

void pdfBoard::zoomOUT() { 
  qDebug() << "ZoomOut:" << zoom;
  if ( zoom > 1 ) zoom --;
  else zoom /= 2;
  if ( zoom == 0 ) zoom = 0.1;
  updateBoard();
}

void pdfBoard::up() { 
  qDebug() << "Up: (" << x <<"," << y << ")";
  y-=zoom*50;
  if ( y < 0 ) y = 0;
  updateBoard();
}

void pdfBoard::down() { 
  qDebug() << "Down: (" << x <<"," << y << ")";
  y+=zoom*50;
  updateBoard();
}

void pdfBoard::left() { 
  qDebug() << "Left: (" << x <<"," << y << ")";
  x-=zoom*50;
  if ( x < 0 ) x = 0;
  updateBoard();
}

void pdfBoard::right() {
  qDebug() << "Right: (" << x <<"," << y << ")";
  x+=zoom*50;
  updateBoard();
}

  

void pdfBoard::nextPage() { 
  if ( currentPage < maxPages -1 ) { 
    currentPage++;
    updateBoard();
  }
}

void pdfBoard::prevPage() { 
  if ( currentPage > 0 ) {
    currentPage--;
    updateBoard();
  }
}

void pdfBoard::updateBoard() { 
     Poppler::Page *pg = pdf->page( currentPage );  
     if ( pg == 0 ) return;
     QImage image = pg->renderToImage( 72*zoom, 72*zoom, x, y, 600, 800 );
     if ( image.isNull() ) return;
     setMinimumSize(600, 800);
     setMaximumSize(600, 800);
     setPixmap( QPixmap::fromImage( image ) );
     show();
     delete pg;
}

void pdfBoard::goTO(int page) { 
  if ( 0 > page || maxPages <= page ) return;
  currentPage=page;
  updateBoard();
}

int main(int argc, char **argv) { 
  QApplication app( argc, argv );

  if ( argc < 2 ) {
    qDebug() << "Usage: " << argv[0] << " pdf-file ";
    return -1;
  }
    
  QString fileName( argv[1] );
  QFile fl( fileName );

  if ( ! fl.exists() ) { 
    qDebug() << "File not found: " << fileName;
    return -1;
  }

  pdfBoard canvas( fileName );

  QAction *quitAct = new QAction( &canvas );
  QAction *nextAct = new QAction( &canvas );
  QAction *prevAct = new QAction( &canvas );

  QAction *zoomInAct = new QAction( &canvas );
  QAction *zoomOutAct = new QAction( &canvas );

  QAction *leftAct = new QAction( &canvas );
  QAction *rightAct = new QAction( &canvas );
  QAction *upAct = new QAction( &canvas );
  QAction *downAct = new QAction( &canvas );

  quitAct->setShortcut((QString) "Ctrl+Q");
  nextAct->setShortcut((QString) "Ctrl+N");
  prevAct->setShortcut((QString) "Ctrl+P");

  zoomInAct->setShortcut((QString) "Ctrl++");
  zoomOutAct->setShortcut((QString) "Ctrl+-");
  
  leftAct->setShortcut((QString) "Left");
  rightAct->setShortcut((QString) "Right");
  upAct->setShortcut((QString) "Up");
  downAct->setShortcut((QString) "Down");


  canvas.addAction(quitAct);
  canvas.addAction(nextAct);
  canvas.addAction(prevAct);

  canvas.addAction(zoomInAct);
  canvas.addAction(zoomOutAct);

  canvas.addAction(leftAct);
  canvas.addAction(rightAct);
  canvas.addAction(upAct);
  canvas.addAction(downAct);


  QObject::connect( quitAct, SIGNAL( triggered() ), &app, SLOT( quit() ) );
  QObject::connect( nextAct, SIGNAL( triggered() ), &canvas, SLOT( nextPage() ) );
  QObject::connect( prevAct, SIGNAL( triggered() ), &canvas, SLOT( prevPage() ) );

  QObject::connect( zoomInAct, SIGNAL( triggered() ), &canvas, SLOT( zoomIN() ) );
  QObject::connect( zoomOutAct, SIGNAL( triggered() ), &canvas, SLOT( zoomOUT() ) );

  QObject::connect( leftAct, SIGNAL( triggered() ), &canvas, SLOT( left() ) );
  QObject::connect( rightAct, SIGNAL( triggered() ), &canvas, SLOT( right() ) );
  QObject::connect( upAct, SIGNAL( triggered() ), &canvas, SLOT( up() ) );
  QObject::connect( downAct, SIGNAL( triggered() ), &canvas, SLOT( down() ) );

  app.exec();

  return 0;
}

#include "testPoppler.moc"

