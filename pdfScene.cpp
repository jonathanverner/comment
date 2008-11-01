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
#include <QtCore/QTemporaryFile>
#include <QtCore/QDebug>
#include <QtCore/QEvent>

#include <poppler-qt4.h>
#include <podofo/podofo.h>

#include "pdfPageItem.h"
#include "abstractTool.h" 
#include "pdfScene.h"
#include "pageBeginItem.h"

pdfScene::pdfScene(): 
	pdf(NULL), tempFileName(""), numPages(0), leftSkip(10), pageSkip(10)
{
  setBackgroundBrush(Qt::gray);
}

pdfScene::pdfScene( const QSet<abstractTool *> &tools, QString fName ):
	tools(tools), pdf(NULL), tempFileName(""), numPages(0), leftSkip(10), pageSkip(10)
{
  setBackgroundBrush(Qt::gray);
  if ( fName != "" ) loadFromFile( fName );
}


void pdfScene::registerTool( abstractTool *tool ) {
  tools.insert( tool );
}

void pdfScene::processPage( PoDoFo::PdfDocument *pdf, int pgNum ) { 
  PoDoFo::PdfPage *pg = pdf->GetPage( pgNum );
  abstractAnnotation *annot;

  /* The following piece of code iterates through the
   * list of annotations and whenever one of the tools
   * recognizes the annotation, it is 
   *  1) converted into the toolAnnotation (i.e. some descendant of abstractAnnotation)
   *     and added to the internal list of annotations on this page
   *  2) deleted from the PoDoFo::PdfPage
   *
   * However, when we delete an annotation from the page, the
   * indexing of the annotations changes so we cannot do a
   * straightforward loop through the annotations. 
   */ 
  int num_of_retained=0;
  bool retainCurAnnot;
  while( pg->GetNumAnnots() > num_of_retained ) { 
    retainCurAnnot=true;
    foreach( abstractTool *tool, tools.toList() ) {
      if ( annot = tool->processAnnotation( pg->GetAnnotation( num_of_retained ) ) ) { 
	try {
	  pg->DeleteAnnotation( num_of_retained );
	  annotations[pgNum].append( annot );
	  retainCurAnnot = false;
	} catch ( PoDoFo::PdfError error ) { 
	  qDebug() << "Cannot delete annotation, not leaving it alone.";
	}
	break;
      }
    }
    if ( retainCurAnnot ) num_of_retained++;
  }
}

// assumes pdf == NULL ( otherwise there will be a memory leak ! )
void pdfScene::loadPopplerPdf( QString fileName, QObject *pageInViewReceiver, const char *slot ) { 
  QPointF annotationPos;
  pdf = Poppler::Document::load( fileName );
  if ( ! pdf ) return;
  pdf->setRenderHint( Poppler::Document::TextAntialiasing, true );
  pdf->setRenderHint( Poppler::Document::Antialiasing, true );
  pdfPageItem *pageItem;
  pageBeginItem *beginMarker;
  qreal y=pageSkip;
  for(int i = 0; i < numPages; i++ ) {
    pageItem = new pdfPageItem( pdf->page( i ) );
    addItem( pageItem );
    pageItem->setPos(leftSkip,y);
    pageCorners.append( QPointF( leftSkip, y ) );
    QRectF pgSize = pageItem->boundingRect();
    // note that passing the third parameter already
    // adds beginMarker to the scene, so we need not add
    // it again;
    beginMarker = new pageBeginItem( i+1, pgSize.width(), pageItem ); // i+1 since pageBeginItem expects page numbers to start from 1
    beginMarker->setPos(0, pgSize.height()/2);
    y+=pageItem->boundingRect().height()+pageSkip;
    addPageAnnotations( i, pageItem );
    qDebug() << "Page Size:" << pageItem->boundingRect();
    qDebug() << i;
    connect( beginMarker->getSignalEmitter(), SIGNAL( pageInView(int) ), pageInViewReceiver, slot );
  }
}

// pageNum is zero-based
void pdfScene::addPageAnnotations( int pageNum, QGraphicsItem *pageItem ) { 
  if ( pageNum < annotations.size() ) { 
    qreal x,y,w,h;
    foreach( abstractAnnotation *a, annotations[pageNum] ) { 
      a->setParentItem( pageItem );
      a->setZValue( 10 );
//      pageItem->boundingRect()->
      a->setPos( QPointF( a->pos().x(), pageItem->boundingRect().height() - a->pos().y() ) );
      qDebug() << "Annotation QPosF" << a->scenePos();

    }
  }
}

void pdfScene::generateTempFileName() { // FIXME: can fail !!!
  QTemporaryFile fl;
  fl.open();
  tempFileName = QFile::encodeName( fl.fileName() );
  return;
}

bool pdfScene::loadFromFile( QString fileName, QObject *pageInViewReceiver, const char *slot ) {
  if ( !QFile::exists(fileName) ) return false;
  QByteArray flnm = QFile::encodeName( fileName );
  PoDoFo::PdfMemDocument pdfDoc;
  try { 
    pdfDoc.Load( flnm.data() );
  } catch ( PoDoFo::PdfError error ) { 
    qDebug() << "Error loading file:" << error.what();
    return false;
  }
  numPages = pdfDoc.GetPageCount();
  annotations.resize( numPages );
  for(int i = 0; i< numPages; i++ ) processPage( &pdfDoc, i );
  if ( tempFileName == "" ) generateTempFileName(); // FIXME: may fail !!!
  pdfDoc.Write( tempFileName.data() );
  loadPopplerPdf( tempFileName, pageInViewReceiver, slot );
  myFileName = fileName;
  return true;
}



bool pdfScene::saveToFile( QString fileName ) {
  PoDoFo::PdfMemDocument pdfDoc;
  abstractAnnotation *a;
  try { 
    pdfDoc.Load( tempFileName );
  } catch ( PoDoFo::PdfError error ) { 
    qDebug() << "Error loading file:" << error.what();
    return false;
  } 
  foreach( QGraphicsItem *item, items() ) { 
    if ( a = dynamic_cast< abstractAnnotation *>(item) )
      a->saveToPdfPage( &pdfDoc, posToPage( a->scenePos() ) );
  }
  pdfDoc.Write( QFile::encodeName( fileName ).data() );
  return true;
}

bool pdfScene::save() {
  saveToFile( myFileName );
}

int pdfScene::posToPage( const QPointF &scenePos ) {
  /* Just in case we have numPages > pageCorners.size();
   * (This can happen when loading a file, because
   *  we initialize numPages right away, but only
   *  initialize pageCorners after some processing */
  int max = pageCorners.size();  
  qreal y=pageSkip, limit = scenePos.y();
  for(int i=0;i<max;i++) {
    if ( y >= limit ) return i;
    y+=pageCorners[i].y();
  }
  if ( max > 1 ) return max-1;
  return 0;
}


QPointF pdfScene::topLeftPage( int page ) {
  if ( page < pageCorners.size() ) return pageCorners[page];
  return QPointF(0,0);
}

//void pdfScene::event( QEvent *e ) {
//};
