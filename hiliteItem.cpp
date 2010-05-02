/***************************************************************
 * hiliteItem.cpp
 * @Author:      Jonathan Verner (jonathan.verner@matfyz.cz)
 * @License:     GPL v2.0 or later
 * @Created:     2009-02-13.
 * @Last Change: 2009-02-13.
 * @Revision:    0.0
 * Description:
 * Usage:
 * TODO:
 *CHANGES:
 ***************************************************************/


#include "hiliteItem.h"

#include <QtCore/QDebug>

#include <poppler-qt4.h>

using namespace Poppler;

hiliteItem::hiliteItem( QPointF topLeftPage, QList<TextBox *> bboxes ):
	col( 0, 255, 217, 100 ), bBox( 0, 0, 0, 0 ), active(false)
{ 
  setPos( topLeftPage );
  updateBBoxes( bboxes );
}

hiliteItem::hiliteItem():
	col( 0, 0, 0, 100), bBox( 0, 0, 0, 0 ), active(false)
{
  setPos( QPointF(0,0) );
  hide();
}

void hiliteItem::setColor( QColor cl ) { 
  col = cl;
  update();
}

void hiliteItem::setActive() {
  active=true;
  col = QColor( 0, 255, 217, 100 );
  update();
}

void hiliteItem::setInactive() {
  active=false;
  col = QColor( 105, 255, 210, 100);
  update();
}

QRectF hiliteItem::boundingRect() const { 
  return bBox;
}

void hiliteItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget ) {
  QPointF myPos = pos();
  foreach( QRectF box, hBoxes ) { 
    painter->fillRect( box, col );
    if (active) painter->drawRect( box );
  }
}

QPainterPath hiliteItem::shape() const { 
  return exactShape;
}

void hiliteItem::clear() { 
  hide();
  update();
  prepareGeometryChange();
  setPos(QPointF(0,0));
  hBoxes.clear();
  bBox = QRectF(0,0,0,0);
  exactShape = QPainterPath();
  update();
}

void hiliteItem::updateBBoxes( QList<TextBox*> bboxes ) { 
  QPainterPath tmp;
  QRectF br;
  update();
  prepareGeometryChange();
  hBoxes.clear();
  foreach( TextBox *box, bboxes ) { 
    br = box->boundingBox();
    hBoxes.append( br );
    tmp.addRect( br );
  }
  bBox = tmp.boundingRect();
  exactShape=tmp;
  update();
}
