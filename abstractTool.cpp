/***************************************************************
 * abstractTool.cpp
 * @Author:      Jonathan Verner (jonathan.verner@matfyz.cz)
 * @License:     GPL v2.0 or later
 * @Created:     2008-10-17.
 * @Last Change: 2008-10-17.
 * @Revision:    0.0
 * Description:
 * Usage:
 * TODO:
 *CHANGES:
 ***************************************************************/
#include <QtGui/QStackedWidget>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QGraphicsSceneHoverEvent>

#include <QtCore/QDebug>

#include <podofo/podofo.h>

#include "abstractTool.h"
#include "myToolTip.h"

/* Called by an item, which wants to be edited. The item passes
 * * a reference to itself */
 void abstractTool::editItem( abstractAnnotation *item ) {
  currentEditItem = item;
  editArea->setCurrentWidget( editor );
  editArea->show();
}


abstractAnnotation::abstractAnnotation( abstractTool *tool, int pg, PoDoFo::PdfAnnotation *annotation ):
	myTool( tool ), date( QDate::currentDate() ), time( QTime::currentTime() ) page( pg ) {
}

void abstractAnnotation::setMyToolTip(const QPixmap &pixMap) {
  setAcceptsHoverEvents(true);
  haveToolTip = true;
  tp = pixmap;
  toolTipPixMap = pixMap;
  if ( showingToolTip ) myToolTip::update( pixMap );
}

void abstractAnnotation::setMyToolTip(const QString &richText) { 
  setAcceptsHoverEvents(true);
  haveToolTip = true;
  tp = text;
  toolTipRichText = richText;
  if ( showingToolTip ) myToolTip::update( richText );
}

void abstractAnnotation::mouseMoveEvent( QGraphicsSceneMouseEvent *event ) {
  moved = true;
  qreal dx = event->pos().x() - event->lastPos().x();
  qreal dy = event->pos().y() - event->lastPos().y();
  setPos( pos().x() + dx, pos().y() + dy );
  qDebug() << "Mouse moved";
}

void abstractAnnotation::mouseReleaseEvent( QGraphicsSceneMouseEvent *event ) { 
  qDebug() << "Mouse released (moved = "<<moved<<")";
  if ( moved ) { 
    moved = false;
  } else myTool->editItem( this );
}


void abstractAnnotation::mousePressEvent( QGraphicsSceneMouseEvent *event ) {
   moved = false;
   event->accept();
   qDebug() << "Mouse pressed";
}

void abstractAnnotation::hoverEnterEvent( QGraphicsSceneHoverEvent *event ) {
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

void abstractAnnotation::hoverLeaveEvent( QGraphicsSceneHoverEvent *event ) {
  qDebug() << "Mouse hover LEAVE";
  myToolTip::hide();
  showingToolTip = false;
}

void abstractAnnotation::setIcon(QPixmap &icn) {
  if ( ! ( icn.rect() == icon.rect() ) ) prepareGeometryChange();
  icon = icn;
  update( icon.rect() );
}

QRectF abstractItem::boundingRect() const {
  return (QRectF) icon.rect();
}

void abstractItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget ) {
  painter->drawPixmap( option->exposedRect, icon, option->exposedRect );

}









