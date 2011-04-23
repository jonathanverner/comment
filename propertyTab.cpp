/**  This file is part of project comment
 *
 *  File: propertyTab.cpp
 *  Created: 2009-01-07
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



#include "propertyTab.h"


#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QColorDialog>
#include <QtGui/QPixmap>
#include <QtGui/QIcon>

propertyTab::propertyTab( QWidget *parent) : QWidget( parent ) {
  layout = new QVBoxLayout;
  authorEdit = new QLineEdit;
  colorPush = new QPushButton;
  pix = new QPixmap(20,10);
  pix->fill(QColor(Qt::black));
  colorPush->setIcon(QIcon(*pix));
  
  
  addWidgetWithLabel( "Author", authorEdit );
  addWidgetWithLabel( "Color", colorPush );
  setLayout( layout );
  connect( authorEdit, SIGNAL( textChanged( const QString & ) ), this, SIGNAL( authorChanged() ) );
  connect( colorPush, SIGNAL( pressed() ), this, SLOT( inputColor() ) );
}

void propertyTab::addWidgetWithLabel( QString Label, QWidget *w ) { 
  QHBoxLayout *tempLayout = new QHBoxLayout;
  tempLayout->addWidget( new QLabel( Label ) );
  tempLayout->addWidget( w );
  layout->addLayout( tempLayout );
}

void propertyTab::setAuthor( QString Author ) {
  authorEdit->setText( Author );
}

void propertyTab::inputColor() {
  QColor tmp_col = QColorDialog::getColor( getColor(), this, QString("Select a color"));
  if ( tmp_col.isValid() && getColor() != tmp_col ) {
    setColor( tmp_col );
    emit colorChanged();
  }
}

void propertyTab::setColor( QColor col ) {
  if ( col.isValid() ) color = col;
  pix->fill(col);
  colorPush->setIcon(QIcon(*pix));
}

QString propertyTab::getAuthor() {
  return authorEdit->text();
}

QColor propertyTab::getColor() {
  return color;
}

#include "propertyTab.moc"
