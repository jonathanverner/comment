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
#include <QtGui/QAction>
#include <QtGui/QToolBar>
#include <QtGui/QGraphicsView>
#include <QtGui/QVBoxLayout>
#include <poppler-qt4.h>

#include "mainWindow.h"
#include "pdfPageItem.h"

void mainWindow::createActions() {
  nextAct = new QAction(QIcon(":next.png"), tr("&Next Page"), this);
  prevAct = new QAction(QIcon(":prev.png"), tr("&Previous Page"), this);
  pageNums = new QLineEdit( this );
}

void mainWindow::createToolBar() { 
  toolBar = new QToolBar( this );
  toolBar->addAction( prevAct );
  toolBar->addWidget( pageNums );
  toolBar->addAction( nextAct );
}
void mainWindow::createOther() {
  scene = new QGraphicsScene(this);
  pageView = new QGraphicsView( scene );
  editor = new QWidget( this );
  pdfPageItem *pdfPage = new pdfPageItem( pdf->page(0) );
  scene->addItem( pdfPage );
}

void mainWindow::assemble() {
  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget( toolBar );
  mainLayout->addWidget( pageView );
  mainLayout->addWidget( editor );
  setLayout( mainLayout );
}

mainWindow::mainWindow(Poppler::Document *PDF): pdf(PDF) {
  createOther();
  createActions();
  createToolBar();
  assemble();
}

