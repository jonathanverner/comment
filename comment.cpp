/***************************************************************
 * comment.cpp
 * @Author:      Jonathan Verner (jonathan.verner@matfyz.cz)
 * @License:     GPL v2.0 or later
 * @Created:     2008-10-16.
 * @Last Change: 2008-10-16.
 * @Revision:    0.0
 * Description:
 * Usage:
 * TODO:
 *CHANGES:
 ***************************************************************/




#include <QtGui/QApplication>
#include <QtCore/QString>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include "mainWindow.h"

int main(int argc, char **argv) { 
  QApplication app( argc, argv );

  if ( argc < 2 ) {
    qDebug() << "Usage: " << argv[0] << " pdf-file";
    return -1;
  }
    
  QString fileName( argv[1] );
  QFile fl( fileName );
  if ( ! fl.exists() ) { 
    qDebug() << "File not found: " << fileName;
    return -1;
  }
  
  mainWindow mW;
  if (! mW.loadFile( fileName ) ) { 
    qDebug() << "Error reading file " << fileName;
    return -1;
  }

  mW.show();
  QObject::connect( &mW, SIGNAL( quit() ), &app, SLOT( quit() ) );
  app.exec();

  return 0;
}


 
 
