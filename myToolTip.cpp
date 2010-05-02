/***************************************************************
 * myToolTip.cpp
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
