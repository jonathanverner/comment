/**  This file is part of project comment
 *
 *  File: searchBar.cpp
 *  Created: 2009-01-31
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
