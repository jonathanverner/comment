/***************************************************************
 * abstractPageItem.cpp
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
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QGraphicsSceneHoverEvent>
#include <QtCore/QDebug>
 
#include "abstractPageItem.h"
#include "myToolTip.h"

void abstractPageItem::setMyToolTip(const QPixmap &pixMap) {
  setAcceptsHoverEvents(true);
  haveToolTip = true;
  tp = pixmap;
  toolTipPixMap = pixMap;
  if ( showingToolTip ) myToolTip::update( pixMap );
}

void abstractPageItem::setMyToolTip(const QString &richText) { 
  setAcceptsHoverEvents(true);
  haveToolTip = true;
  tp = text;
  toolTipRichText = richText;
  if ( showingToolTip ) myToolTip::update( richText );
}

void abstractPageItem::mouseMoveEvent( QGraphicsSceneMouseEvent *event ) {
  moved = true;
  qreal dx = event->pos().x() - event->lastPos().x();
  qreal dy = event->pos().y() - event->lastPos().y();
  setPos( pos().x() + dx, pos().y() + dy );
  qDebug() << "Mouse moved";
}

void abstractPageItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *event ) { 
  qDebug() << "Mouse released (moved = "<<moved<<")";
  if ( moved ) { 
    moved = false;
  } else if ( editForm ) editForm->editItem( this );
}


void abstractPageItem::mousePressEvent( QGraphicsSceneMouseEvent *event ) {
   moved = false;
   event->accept();
   qDebug() << "Mouse pressed";
}

void abstractPageItem::hoverEnterEvent( QGraphicsSceneHoverEvent *event ) {
  qDebug() << "Mouse hover ENTER";
  if ( haveToolTip ) { 
    switch( tp ) { 
	    case pixmap:
		    myToolTip::showPixmap(event->screenPos(), toolTipPixMap );
		    showingToolTip = true;
		    break;
	    case text:
		    myToolTip::showText(event->screenPos(), toolTipRichText );
		    showingToolTip = true;
		    break;
    }
  }
}

void abstractPageItem::hoverLeaveEvent( QGraphicsSceneHoverEvent *event ) {
  qDebug() << "Mouse hover LEAVE";
  myToolTip::hide();
  showingToolTip = false;
}


