/***************************************************************
 * pageEndItem.cpp
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


#include "pageBeginItem.h"




pageBeginItem::pageBeginItem( int pgNum, qreal w, QGraphicsItem *parent ) : QGraphicsItem(parent), pageNum( pgNum ), wd(w) { 
  sg = new pageBeginSignalEmitter();
}

pageBeginItem::~pageBeginItem() {
}



void pageBeginItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget ) {
  sg->emitSignal( pageNum );
}




#include "pageBeginItem.moc" 
