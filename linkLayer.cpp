#include "linkLayer.h"

#include "pdfUtil.h"
#include "pdfScene.h"

#include <podofo/podofo.h>
#include <QtCore/QSignalMapper>
#include <QAction>

using namespace PoDoFo;

linkLayer::linkLayer(pdfScene* sc, PdfDocument* doc):
  sceneLayer(sc)
{
  loadOutlineItem( doc->GetOutlines(), NULL, "toc" );
  mapper = new QSignalMapper( this );
  connect( mapper, SIGNAL( mapped(const QString &) ), this, emitGOTO( const QString & ) );
}


void linkLayer::emitGOTO(const QString& name) {
  linkItem *item = getTarget( name );
  emit gotoTarget( item );
}


linkItem *linkLayer::addTOCItem( QString &title, linkItem *parent, linkItem *tgt ) {
  linkItem *tocItem = new linkItem( QRectF(0,0,0,0), title );
  tocItem->setTarget( tgt );
  QAction *activate = new QAction( title, tocItem ); 
  connect( activate, SIGNAL(triggered(bool)), mapper, SLOT(map()) );
  mapper->setMapping( activate, tgt->getName() );
  return tocItem;
};

linkItem *linkLayer::loadOutlineItem( PdfOutlineItem *item, linkItem *parent, const QString &path ) { 
  pdfScene *pSc = dynamic_cast<pdfScene>(scene);
  QString title = pdfUtil::pdfStringToQ( item->GetTitle() );
  QString tgtName = path + "/" + title;
  PdfDestination *dest = item->GetDestination();
  linkItem *tgt = addTarget( tgtName, pSc->topLeftPage( dest->GetPage()->GetPageNumber()-1) );
  linkItem *me = addTOCItem( title, parent, tgt );
  PdfOutlineItem *child = item->First();
  while( child ) { 
    loadOutlineItem( child, me, tgtName );
    child = child->Next();
  }
}


