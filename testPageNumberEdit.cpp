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
#include <QtGui/QToolBar>

#include "pageNumberEdit.h"
int main( int argc, char **argv ) { 
  QApplication app( argc, argv );
  QMainWindow mainWin;
  QToolBar *tool = new QToolBar( &mainWin );
  pageNumberEdit *pageEdit = new pageNumberEdit( &mainWin ), *pg2 = new pageNumberEdit( &mainWin );
  pg2->setMaxPageNum(4520);
  pg2->setPageNumber(100);
  pageEdit->setMaxPageNum(100);
  pageEdit->setPageNumber( 1 );
  tool->addWidget( pageEdit );
  tool->addWidget( pg2 );
  mainWin.addToolBar( tool );
  mainWin.show();
  app.exec();
}
  

 
