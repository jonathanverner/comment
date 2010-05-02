
#include "linkLayer.h"
#include "pdfScene.h"


#include <QtCore/QSignalMapper>
#include <QAction>

void targetItem::activate()
{
  emit activated();
}




linkLayer::linkLayer(pdfScene* sc):
  sceneLayer(sc)
{
  mapper = new QSignalMapper( this );
  connect( mapper, SIGNAL(mapped(const QString &)), this, SLOT(emitGOTO(const QString &)) );
}

targetItem* linkLayer::addTarget ( const QString& name, const QRectF& target ) {
  if ( targets.contains( name ) ) return targets[name];
  QRectF area = QRectF(QPointF(0,0),target.size());
  targetItem *tgt = new targetItem( area, name );
  connect( tgt, SIGNAL(activated()), mapper, SLOT(map()));
  mapper->setMapping( tgt, name );
  targets.insert( name, tgt );
  addItem( tgt );
  tgt->setPos( target.topLeft() );
}

targetItem* linkLayer::addTarget ( const QString& name, const int page, const QRectF& target )
{
  if ( targets.contains( name ) ) return targets[name];
  QRectF area = QRectF(QPointF(0,0),target.size());
  targetItem *tgt = new targetItem( target, name );
  connect( tgt, SIGNAL(activated()), mapper, SLOT(map()));
  mapper->setMapping( tgt, name );
  targets.insert( name, tgt );
  addItem( tgt );
  pdfScene *sc = dynamic_cast<pdfScene *>(scene);
  tgt->setPos(target.topLeft()+sc->topLeftPage(page));
}

void linkLayer::removeTarget ( const QString& name ) {
  if ( ! targets.contains( name ) ) return;
  targetItem *tgt = targets[name];
  delete tgt;
  targets.remove(name);
}



void linkLayer::emitGOTO(const QString& name) {
  if ( targets.contains( name ) ) emit gotoTarget( targets[name] );
}



#include "linkLayer.moc"
