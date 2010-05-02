/***************************************************************
 * sceneLayer.cpp
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

 
#include "sceneLayer.h"

#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsItem>

#include <QtCore/QDebug>

sceneLayer::sceneLayer( QGraphicsScene *SC):
	scene(SC), cItem(0), zVal(0)
{
}

void sceneLayer::setZValue( int zV ) { 
  zVal = zV;
  foreach( QGraphicsItem *it, items ) { 
    it->setZValue( zVal );
  }
}

void sceneLayer::addItem( QGraphicsItem *item, bool addToScene) {
  items.append(item);
  reference.append( ! addToScene );
  if ( addToScene ) scene->addItem( item );
  item->setZValue( zVal );
}

QGraphicsItem *sceneLayer::currentItem() {
  Q_ASSERT( cItem >= 0 );
  if ( cItem < items.size() ) return items[cItem];
  else return NULL;
}

void sceneLayer::advanceCurrentItem( int i ) {
  if ( items.size() <= 0 ) return;
  cItem+=i;
  while ( cItem >= items.size() ) cItem-=items.size();
  while ( cItem < 0 ) cItem +=items.size();
}

void sceneLayer::hide() {
  foreach( QGraphicsItem *it, items ) { 
    it->hide();
  }
}

void sceneLayer::show() {
  foreach( QGraphicsItem *it, items ) { 
    it->show();
  }
}

void sceneLayer::clear() {
  for(int i=0; i < items.size(); i++ ) { 
    if ( ! reference[i] ) {
      scene->removeItem( items[i] );
      delete items[i];
    }
  }
  items.clear();
  reference.clear();
  cItem=0;
}

#include "sceneLayer.moc"
