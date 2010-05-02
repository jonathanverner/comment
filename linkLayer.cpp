#include "linkLayer.h"
#include "linkLayer_p.h"

#include "pdfUtil.h"
#include "pdfScene.h"

#include <podofo/podofo.h>
#include <QtCore/QSignalMapper>
#include <QAction>

using namespace PoDoFo;




linkLayer::linkLayer(pdfScene* sc, PdfDocument* doc):
  sceneLayer(sc)
{
  mapper = new QSignalMapper( this );
  connect( mapper, SIGNAL( mapped(const QString &) ), this, emitGOTO(const QString &) );
}


void linkLayer::emitGOTO(const QString& name) {
  if ( targets.contains( name ) ) emit gotoTarget( targets[name] );
}



#include "linkLayer.moc"
#include "linkLayer_p.moc"
