/**  This file is part of project comment
 *
 *  File: comment.cpp
 *  Created: 2008-10-16
 *  Author: Jonathan Verner <jonathan.verner@matfyz.cz>
 *  License: GPL v2 or later
 *
 *  Copyright (C) 2010 Jonathan Verner <jonathan.verner@matfyz.cz>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 */





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


 
 
