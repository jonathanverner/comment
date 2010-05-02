/***************************************************************
 * wordItem.cpp
 * @Author:      Jonathan Verner (jonathan.verner@matfyz.cz)
 * @License:     GPL v2.0 or later
 * @Created:     2008-11-07.
 * @Last Change: 2008-11-07.
 * @Revision:    0.0
 * Description:
 * Usage:
 * TODO:
 *CHANGES:
 ***************************************************************/

#include "wordItem.h"

#include <QtGui/QPainter>
#include <poppler-qt4.h> 

using namespace Poppler;

wordItem::wordItem( TextBox *box ): bx(box), hilited(false) {
  setZValue(5);
  w=bx->boundingBox().width();
  h=bx->boundingBox().height();
  setPos( bx->boundingBox().x(), bx->boundingBox().y() );
};

void wordItem::hilight( bool on ) {
  hilited = on;
}

QString wordItem::word() {
  return bx->text();
}

QRectF wordItem::boundingRect() const {
  return QRectF( 0,0,w,h );
}
void wordItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget ) {
  if (hilited) { 
    painter->fillRect(QRectF(0,0,w,h)/* bx->boundingBox()*/, QColor(255,255,0,128) );
  } else {
//    painter->drawRect(QRectF(0,0,w,h)/* bx->boundingBox()*/);
  }
}

