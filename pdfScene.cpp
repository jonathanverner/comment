/***************************************************************
 * pdfScene.cpp
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

#include <QtCore/QFile>

#include <poppler-qt4.h>
#include <podofo/podofo.h>

#include "pdfPageItem.h"
#include "abstractTool.h" 
#include "pdfScene.h"

pdfScene::pdfScene(): 
	pdf(NULL), tempFileName(""), numPages(0), leftSkip(10), pageSkip(10)
{
  setBackgroundBrush(Qt::gray);
}

pdfScene::pdfScene( const QList<abstractTool *> &tools ):
	tools(tools), pdf(NULL), tempFileName(""), numPages(0), leftSkip(10), pageSkip(10)
{
  setBackgroundBrush(Qt::gray);
}

pdfScene::pdfScene( const QList<abstractTool *> &tools, QString fileName ):
	tools(tools), pdf(NULL), tempFileName(""), numPages(0), leftSkip(10), pageSkip(10)
{
  setBackgroundBrush(Qt::gray);
  loadFromFile( fileName );
}

void pdfScene::registerTool( abstractTool *tool ) {
  tools.insert( tool );
}

void pdfScene::processPage( PoDoFo::PdfDocument *pdf, int pgNum ) { 
  PoDoFo::PdfPage *pg = pdf->GetPage( pgNum );
  int numAnnots = pg->GetNumAnnots();
  for(int i = 0; i< numAnnots; i++ ) { 
    foreach( abstractTool *tool, tools.toList() ) { 
      if ( tool->processAnnotation( pg->GetAnnotation( i ), pgNum ) ) { 
	pg->DeleteAnnotation( i );
	break;
      }
    }
  }
}

// assumes pdf == NULL ( otherwise there will be a memory leak ! )
void pdfScene::loadPopplerPdf( QString fileName ) { 
  pdf = Poppler::Document::load( fileName );
  if ( ! pdf ) return;
  pdf->setRenderHint( Poppler::Document::TextAntialiasing, true );
  pdf->setRenderHint( Poppler::Document::Antialiasing, true );
  pdfPageItem *pageItem;
  pageBeginItem *beginMarker;
  qreal y=pageSkip;
  for(int i = 0; i < numPages; i++ ) {
    pageItem = new pdfPageItem( pdf->page( i ) );
    pageItem->setPos(leftSkip,y);
    pageCorners.add( QPointF( leftSkip, y ) );
    QRectF pgSize = pageItem->boundingRect();
    beginMarker = new pageBeginItem( i+1, pgSize.width());
    beginMarker->setPos(leftSkip, y+pgSize.height()/2);
    y+=pageItem->boundingRect().height()+pageSkip;
    scene->addItem( pageItem );
    scene->addItem( beginMarker );
    connect( beginMarker->getSignalEmitter(), SIGNAL( pageInView(int) ), pageView, SIGNAL( onPage(int) ) );
  }
}

void generateTempFileName() { // FIXME: can fail !!!
  QTemporaryFile fl;
  fl.open();
  tempFileName = QFile::encodeName( fl.fileName );
  return;
}

bool pdfScene::loadFromFile( QString fileName ) {
  if ( !QFile::.exists(fileName) ) return false;
  QByteArray flnm = QFile::encodeName( fileName );
  PoDoFo::PdfMemDocument pdfDoc;
  try { 
    pdfDoc.load( flnm.data() );
  } catch ( PoDoFo::PdfError error ) { 
    qDebug() << "Error loading file:" << error.what();
    return false;
  }
  numPages = pdfDoc.GetPageCount();
  for(int i = 0; i< numPages; i++ ) processPage( &pdfDoc, i );
  if ( tempFileName == "" ) generateTempFileName(); // FIXME: may fail !!!
  pdfDoc.Write( tempFileName.data() );
  loadPopplerPdf( tempFileName );
  updateAnnotationPositions();
  myFileName = fileName;
  return true;
}

void pdfScene::updateAnnotationPositions() { 
  foreach( QGraphicsItem *item, items() ) { 
    if ( dynamic_cast< abstractAnnotation *> item ) { 
      item->setPos( topLeftPage( item->getPage() ) + item->scenePos() );
    }
  }
} 


bool pdfScene::saveToFile( QString fileName ) {
  PoDoFo::PdfMemDocument pdfDoc;
  try { 
    pdfDoc.load( tempFileName );
  } catch ( PoDoFo::PdfError error ) { 
    qDebug() << "Error loading file:" << error.what();
    return false;
  } 
  foreach( QGraphicsItem *item, items() ) { 
    if ( dynamic_cast< abstractAnnotation *> item )
      item->saveToPdfPage( &pdfDoc, posToPage( item.scenePos() ) );
  }
  pdfDoc.Write( QFile::encodeName( fileName ).data() );
  return true;
}

bool pdfScene::save() {
  saveToFile( myFileName );
}

int pdfScene::posToPage( const QPoint &scenePos ) {
  /* Just in case we have numPages > pageCorners.size();
   * (This can happen when loading a file, because
   *  we initialize numPages right away, but only
   *  initialize pageCorners after some processing */
  int max = pageCorners.size();  
  qreal y=pageSkip, limit = scenePos.y();
  for(int i=0;i<max;i++) {
    if ( y >= limit ) return i;
    y+=pageCorners[i];
  }
  if ( i > 0 ) return i-1;
  return 0;
}


QPoint pdfScene::topLeftPage( int page ) {
  if ( page < pageCorners.size() ) return pageCorners[page];
  return QPoint(0,0);
}

