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
#include <QtGui/QPainter>
#include <QtGui/QStyleOptionGraphicsItem>
#include <QtGui/QLabel>

#include <QtCore/QDebug>

#include <podofo/podofo.h>

#include "abstractTool.h"
#include "myToolTip.h"
#include "pageView.h"

/* Called by an item, which wants to be edited. The item passes
 * * a reference to itself */
void abstractTool::editItem( abstractAnnotation *item ) {
  currentEditItem = item;
  editArea->setCurrentWidget( editor );
  editArea->show();
  editArea->setFocus();
}

bool abstractTool::handleEvent( viewEvent *ev ) { 
  if ( ev->type() == viewEvent::VE_MOUSE_RELEASE && ( ev->btnCaused() == Qt::LeftButton ) ) { 
    QPointF pos=ev->scenePos(), delta = ev->sceneDelta();
    abstractAnnotation *annot = dynamic_cast<abstractAnnotation*>(ev->item());
    if ( ev->isClick() ) { 
      if ( annot ) editItem( annot );
      else newActionEvent( &pos );
      return true;
    }
  } else return false;
}

abstractAnnotation::abstractAnnotation( abstractTool *tool ):
	myTool( tool ), date( QDate::currentDate() ), time( QTime::currentTime() ), haveToolTip(false), showingToolTip(false), movable( true )
{
  setAcceptsHoverEvents( true );
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



bool abstractAnnotation::editSelf() { 
  myTool->editItem( this );
  return true;
}


bool abstractAnnotation::hasToolTip() { 
  if ( haveToolTip ) { 
    if ( tp == text && toolTipRichText.size() < 1 ) return false;
    return true;
  }
  return false;
}

bool abstractAnnotation::showToolTip( const QPoint &scPos ) { 
  qDebug() << "Showing Tooltip";
  if ( haveToolTip ) { 
    switch( tp ) { 
	    case pixmap:
		    myToolTip::showPixmap(scPos, toolTipPixMap );
		    showingToolTip = true;
		    return true;
		    break;
	    case text:
		    if ( toolTipRichText.size() < 1 ) return false;
		    myToolTip::showText(scPos, toolTipRichText );
		    showingToolTip = true;
		    return true;
		    break;
    }
  }
  return false;
}

void abstractAnnotation::hideToolTip() { 
  qDebug() << "Hiding Tooltip";
  myToolTip::hide();
  showingToolTip = false;
  myTool->editArea->hide();
}

void abstractAnnotation::setIcon(const QPixmap &icn) {
  if ( ! ( icn.rect() == icon.rect() ) ) prepareGeometryChange();
  icon = icn;
  update( icon.rect() );
}

QRectF abstractAnnotation::boundingRect() const {
  return (QRectF) icon.rect();
}

void abstractAnnotation::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget ) {
  painter->drawPixmap( option->exposedRect, icon, option->exposedRect );
}








