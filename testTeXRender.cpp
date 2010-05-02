/**  This file is part of project comment
 *
 *  File: testTeXRender.cpp
 *  Created: 2008-10-18
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
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QTextEdit>
#include <QtGui/QLabel>
#include <QtGui/QToolBar>

#include <QtCore/QDebug>

#include "renderTeX.h"
#include "testTeXRender.h"


renderer::renderer( QLabel *l, QTextEdit *t ):
	lbl(l), txt(t)
{
  itemID = r.addItem("");
  connect( &r, SIGNAL(itemReady(int)), this, SLOT(pdfReady(int)) );
}

void renderer::updateTeX() { 
  qDebug() << "Updating...";
  r.updateItem( itemID, txt->toPlainText() ); 
}

void renderer::pdfReady( int i ) { 
  qDebug() << "PdfReady...";
  if ( i == itemID ) { 
    lbl->setPixmap( r.render( itemID, false, 2 ) );
  }
}


int main( int argc, char **argv ) { 
  QApplication app( argc, argv );
  QMainWindow mainWin;
  QWidget *wgt = new QWidget;
  QVBoxLayout *mainLayout = new QVBoxLayout(wgt);
  QTextEdit *textEdit = new QTextEdit( &mainWin );
  QLabel *label = new QLabel( &mainWin );
  QPushButton *btn = new QPushButton( &mainWin );
  renderer rn( label, textEdit );
  QObject::connect( btn, SIGNAL(clicked()), &rn, SLOT(updateTeX()) );
  mainLayout->addWidget( label );
  mainLayout->addWidget( textEdit );
  mainLayout->addWidget( btn );
  mainWin.setCentralWidget( wgt );
  mainWin.show();
  app.exec();
}

#include "testTeXRender.moc"
  

 
