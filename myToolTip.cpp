/**  This file is part of project comment
 *
 *  File: myToolTip.cpp
 *  Created: 2008-10-13
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


#include "myToolTip.h"

#include <QtGui/QToolTip>
#include <QtGui/QPalette>
#include <QtGui/QLabel>
#include <QtCore/QTimer>
#include <QtCore/QDebug>

QLabel *myToolTip::toolTip = NULL;
bool myToolTip::waitingForDelay = false;
shower *myToolTip::sh = NULL;

void myToolTip::init() { 
  toolTip = new QLabel( 0, Qt::ToolTip );
  sh = new shower();

//  toolTip->setForegroundRole(QPalette::ToolTipText);
//  toolTip->setBackgroundRole(QPalette::ToolTipBase);
  toolTip->setPalette(QToolTip::palette());
  toolTip->setFrameStyle(QFrame::NoFrame);
  toolTip->setMargin( 3 );
  toolTip->hide();
}

void myToolTip::wait() {
  qDebug() << "Waiting";
  waitingForDelay = true;
  QTimer::singleShot(100, sh, SLOT(show()));
}

void shower::show() { 
  qDebug() << "finished waiting";
  myToolTip::show();
}

void myToolTip::show() { 
  if ( ! toolTip ) init();
  qDebug() << "Showing tooltip ("<<waitingForDelay<<")";
  if ( waitingForDelay ) toolTip->show();
}


void myToolTip::showText( const QPoint &pos, const QString &text, bool delayed ) {
  if ( ! toolTip ) init();
  toolTip->hide();
  toolTip->setText( text );
  toolTip->move( pos );
  if ( delayed ) wait();
  else toolTip->show();
}

void myToolTip::update( const QString &text ) { 
  if ( ! toolTip ) init();
  toolTip->hide();
  toolTip->setText( text );
  toolTip->show();
}

void myToolTip::update( const QPixmap &pix ) { 
  if ( ! toolTip ) init();
  toolTip->hide();
  toolTip->setPixmap( pix );
  toolTip->show();
}


void myToolTip::showPixmap( const QPoint &pos, const QPixmap &pix, bool delayed ) {
  if ( ! toolTip ) init();
  toolTip->hide();
  toolTip->setPixmap( pix );
  toolTip->move( pos );
  if ( delayed ) wait();
  else toolTip->show();
}

void myToolTip::hide() {
  if ( ! toolTip ) init();
  waitingForDelay = false;
  toolTip->hide();
}

#include "myToolTip.moc"
