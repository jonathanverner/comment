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
#include <QtCore/QFileInfo>
#include "mainWindow.h"
#include "config.h"

int main(int argc, char **argv) { 
  QApplication app( argc, argv );
  QString fileName;
  QFile fl;

  if ( argc < 2 ) { // No filename given
    if ( config().haveKey( "last_opened" ) ) { // Try the last opened file
      fileName = config()["last_opened"];
      fl.setFileName( fileName );
      if ( ! fl.exists() ) { // The last opened file does not exist
	qWarning() << "Usage: " << argv[0] << " pdf-file";
	return -1;
      }      
    } else { // No info about the last opened file
      qWarning() << "Usage: " << argv[0] << " pdf-file";
      return -1;
    }
  } else {
    fileName = argv[1];
    fl.setFileName( fileName );
    if ( ! fl.exists() ) { 
      qWarning() << "File not found: " << fileName;
      return -1;
    }
  }

    
  QFileInfo fInfo( fileName );
  mainWindow mW;
  if (! mW.loadFile( fInfo.absoluteFilePath() ) ) { 
    qDebug() << "Error reading file " << fileName;
    return -1;
  }

  mW.show();
  QObject::connect( &mW, SIGNAL( quit() ), &app, SLOT( quit() ) );
  app.exec();

  config()["last_opened"] = fInfo.absoluteFilePath();
  config()[fInfo.absoluteFilePath()] = config()["current_page"];
  config().removeKey("current_page");
  config().save();

  return 0;
}


 
 
