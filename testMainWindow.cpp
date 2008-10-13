/***************************************************************
 * testMainWindow.cpp
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

#include <poppler-qt4.h>
#include <QtCore/QString>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtGui/QApplication>

#include "mainWindow.h"

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
  Poppler::Document *pdf = Poppler::Document::load( fileName );
  if ( ! pdf ) { 
    qDebug() << "Error reading file " << fileName;
    return -1;
  } else  {
    pdf->setRenderHint( Poppler::Document::TextAntialiasing, true );
    pdf->setRenderHint( Poppler::Document::Antialiasing, true );
  }

  
  mainWindow main( pdf );
  main.show();
  app.exec();
}

 
