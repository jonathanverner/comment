/***************************************************************
 * searchBar.cpp
 * @Author:      Jonathan Verner (jonathan.verner@matfyz.cz)
 * @License:     GPL v2.0 or later
 * @Created:     2009-01-31.
 * @Last Change: 2009-01-31.
 * @Revision:    0.0
 * Description:
 * Usage:
 * TODO:
 *CHANGES:
 ***************************************************************/ 

#include "searchBar.h"

#include <QtGui/QHBoxLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtGui/QAction>


searchBar::searchBar( QWidget *parent ) {
  edit = new QLineEdit( this );
  next = new QPushButton( "Next", this );
  prev = new QPushButton( "Prev", this );
  QHBoxLayout *layout = new QHBoxLayout;
  QLabel *findLabel = new QLabel( tr("Find") );
  QAction *hide = new QAction( this );
  hide->setShortcut( (QString) "Esc" );
  addAction( hide );
  connect( hide, SIGNAL( triggered() ), parent, SLOT( hideEditArea() ) );
  connect( edit, SIGNAL( textChanged(const QString &) ), this, SIGNAL( textChanged(const QString &) ) );
  connect( next, SIGNAL( clicked() ), this, SIGNAL( nextMatch() ) );
  connect( prev, SIGNAL( clicked() ), this, SIGNAL( prevMatch() ) );

  edit->setMinimumWidth(10*edit->minimumSizeHint().width());
  layout->addWidget( findLabel );
  layout->addWidget( edit );
  layout->addWidget( next );
  layout->addWidget( prev );
  setLayout( layout );
}

void searchBar::setText( QString text ) {
  edit->setText(text);
}

void searchBar::setNone() { 
  edit->setStyleSheet("background-color: white");
}
void searchBar::setFound() {
  edit->setStyleSheet("background-color: #6EFF69");
}

void searchBar::setMissed() {
  edit->setStyleSheet("background-color: #FF5659");
}

void searchBar::setNumOfMatches(int n) {
}

void searchBar::focus() { 
  edit->selectAll();
  edit->setFocus();
}



#include "searchBar.moc"
