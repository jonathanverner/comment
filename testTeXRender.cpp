
/***************************************************************
 * testPageNumberEdit.cpp
 * @Author:      Jonathan Verner (jonathan.verner@matfyz.cz)
 * @License:     GPL v2.0 or later
 * @Created:     2008-10-18.
 * @Last Change: 2008-10-18.
 * @Revision:    0.0
 * Description:
 * Usage:
 * TODO:
 *CHANGES:
 ***************************************************************/


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
  

 
