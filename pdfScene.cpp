/**  This file is part of project comment
 *
 *  File: pdfScene.cpp
 *  Created: 2008-10-17
 *  Author: Jonathan Verner <jonathan.verner@matfyz.cz>
 *  License: GPL v2 or later
 *
 *  Copyright (C) 2010 Jonathan Verner <jonathan.verner@matfyz.cz>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 */




#include "pdfPageItem.h"
#include "abstractTool.h" 
#include "pdfScene.h"
#include "pageBeginItem.h"
#include "pageTextLayer.h"
#include "pdfUtil.h"
#include "sceneLayer.h"

#include <QtCore/QFile>
#include <QtCore/QTemporaryFile>
#include <QtCore/QDebug>
#include <QtCore/QEvent>

#include <poppler-qt4.h>
#include <podofo/podofo.h>

using namespace Poppler;

pdfScene::pdfScene(): 
	pdf(NULL), tempFileName(""), numPages(0), leftSkip(10), pageSkip(10), prop(NULL)
{
  setBackgroundBrush(Qt::gray);
}

pdfScene::pdfScene( const QSet<abstractTool *> &tools, QString fName ):
	tools(tools), pdf(NULL), tempFileName(""), numPages(0), leftSkip(10), pageSkip(10),
	prop(NULL)
{
  setBackgroundBrush(Qt::gray);
  if ( fName != "" ) loadFromFile( fName );
}

pdfScene::~pdfScene() { 
  delete prop;
  delete pdf;
  // FIXME: further cleanup needed
  // cleanup sceneLayers
  // cleanup textLayer
  // cleanup annotations
}


void pdfScene::registerTool( abstractTool *tool ) {
  tools.insert( tool );
}

/* Go through the page pgNum, remove all recognized annotations
 * and add them to the annotation list. Nothing is added to the
 * scene, this is done only after we load the poppler document */

void pdfScene::processPage( PoDoFo::PdfDocument *pdf, int pgNum ) { 
  PoDoFo::PdfPage *pg = pdf->GetPage( pgNum );
  abstractAnnotation *annot;
  pdfCoords transform(pg);

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
    try {
    foreach( abstractTool *tool, tools.toList() ) {
      if ( annot = tool->processAnnotation( pg->GetAnnotation( num_of_retained ), &transform ) ) { 
	try {
	  pg->DeleteAnnotation( num_of_retained );
	  annotations[pgNum].append( annot );
	  retainCurAnnot = false;
	} catch ( PoDoFo::PdfError error ) {
	  qWarning() << "Cannot delete annotation, not leaving it alone.";
	}
	break;
      }
	
    }
    } catch ( PoDoFo::PdfError error ) {
	qWarning() << "Cannot process annotation:" << error.what();
	retainCurAnnot = true;
    }
    if ( retainCurAnnot ) num_of_retained++;
  }
}

/* Loads a pdf into the scene: 
 *    Goes through the pdf pages, converts each to a pdfPageItem
 *    and adds it to the scene. Also adds all annotations
 *    from the annotations list belonging to the page onto the scene.
 *    Populates the pageCorners list. For each page it adds a pageMarker 
 *    item onto the pageItem, (vertically into the middle). When this pageMarker receives
 *    a paint event, it does nothing but emits a signal (which
 *    is connected to pageInViewReceiver if not NULL). This is how
 *    a view knows which page is currently in view. Note that
 *    if there are different views watching the same scene, this
 *    will confuse the hell out of them :-) We need to eventually
 *    move this stuff into the pageView class, but I currently don't
 *    have the motivation, since in the near future there will always
 *    be only a single view */
// assumes pdf == NULL ( otherwise there will be a memory leak ! )
void pdfScene::loadPopplerPdf( QString fileName, QObject *pageInViewReceiver, const char *slot ) { 
  QPointF annotationPos;
  pdf = Poppler::Document::load( fileName );
  if ( ! pdf ) return;
  pdf->setRenderHint( Poppler::Document::TextAntialiasing, true );
  pdf->setRenderHint( Poppler::Document::Antialiasing, true );
  pdfPageItem *pageItem;
  pageBeginItem *beginMarker;
  pageCorners.clear();
  qreal y=pageSkip;
//  wordItem *it;
  for(int i = 0; i < numPages; i++ ) {
    pageItem = new pdfPageItem( pdf->page( i ) );
    pageItem->setPageNum( i );
    pageItem->setZValue( 0 );
    addItem( pageItem );
    pageItem->setPos(leftSkip,y);
    pageCorners.append( QPointF( leftSkip, y ) );
    qDebug() << "Page("<<i<<"):"<<y<<" == "<<pageCorners[i].y();
    QRectF pgSize = pageItem->boundingRect();
    // note that passing the third parameter already
    // adds beginMarker to the scene, so we need not add
    // it again;
    beginMarker = new pageBeginItem( i+1, pgSize.width(), pageItem ); // i+1 since pageBeginItem expects page numbers to start from 1
    beginMarker->setPos(0, pgSize.height()/2);
    y+=pageItem->boundingRect().height()+pageSkip;
//    QList<poppler::TextBox*> textList = pageItem->getPage()->textList();
/*    foreach( Poppler::TextBox *word, pageItem->getPage()->textList() ) { // add the text layer
      it = new wordItem( word );
      it->setParentItem( pageItem );
    }*/
    textLayer.append( new pageTextLayer( pageItem->getPage() ) );
//    txt = new textLayer( pageItem->getPage() );
//    txt->setParentItem( pageItem );
    addPageAnnotations( i, pageItem );
    qDebug() << "Page Size:" << pageItem->boundingRect();
    qDebug() << i;
    connect( beginMarker->getSignalEmitter(), SIGNAL( pageInView(int) ), pageInViewReceiver, slot );
  }
}

/* Iterates through the annotations on page pageNum and adds them 
 * to the scene by setting their parent to pageItem. Note this
 * requires the annotations to have their positions relative to 
 * the page they belong to. Called from loadPoppler.*/
// pageNum is zero-based
void pdfScene::addPageAnnotations( int pageNum, QGraphicsItem *pageItem ) { 
  if ( pageNum < annotations.size() ) { 
    foreach( abstractAnnotation *a, annotations[pageNum] ) { 
      a->setParentItem( pageItem );
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
  annotations.clear();
  myFileName = fileName;
  prop = new pdfProperties;
  fillPdfProperties();
  return true;
}



bool pdfScene::saveToFile( QString fileName ) {
  PoDoFo::PdfMemDocument pdfDoc;
  PoDoFo::PdfPage *pg;
  pdfPageItem *pgItem;
  pdfCoords coords;
  abstractAnnotation *a;
  try { 
    pdfDoc.Load( tempFileName );
  } catch ( PoDoFo::PdfError error ) { 
    qDebug() << "Error loading file:" << error.what();
    return false;
  } 
  foreach( QGraphicsItem *item, items() ) { 
    if ( a = dynamic_cast< abstractAnnotation *>(item) ) {
      pgItem = dynamic_cast<pdfPageItem*>(a->parentItem());
      if ( ! pgItem ) { 
	qWarning() << "Annotation does not have a parent page, adding to page 0...";
	pgItem=getPageItem(0);
	if ( !pgItem ) {
	  qWarning() << "Page 0 missing, this is really WEIRD!!!";
	  continue;
	}
      }
      qDebug() << " Saving annotation on page " << pgItem->getPageNum();
      pg = pdfDoc.GetPage( pgItem->getPageNum() );
      coords.setPage( pg );
      a->saveToPdfPage( &pdfDoc, pg, &coords );
    }
  }
  savePdfProperties( &pdfDoc );
  pdfDoc.Write( QFile::encodeName( fileName ).data() );
  return true;
}

void pdfScene::savePdfProperties( PoDoFo::PdfMemDocument *doc ) {
  PoDoFo::PdfInfo *info = doc->GetInfo();
  info->SetAuthor( pdfUtil::qStringToPdf( prop->author ) );
  info->SetTitle( pdfUtil::qStringToPdf( prop->title ) );
  info->SetSubject( pdfUtil::qStringToPdf( prop->subject ) );
  info->SetKeywords( pdfUtil::qStringToPdf( prop->keywords ) );
};


void pdfScene::setPdfProperties( struct pdfProperties& p ) { 
  *prop = p;
  return;
}

void pdfScene::getPdfProperties( struct pdfProperties& p ) { 
  p = *prop;
  return;
}

void pdfScene::fillPdfProperties() { 
  prop->author=pdf->info("Author");
  prop->title=pdf->info("Title");
  prop->subject=pdf->info("Subject");
  prop->keywords=pdf->info("Keywords");
  return;
}

/* FIXME: For safety reasons first save the file to a temporary 
 * location and try to load it and do some sanity checks on it. 
 * If it does not load, return false otherwise move it to myFilename. 
 * */
bool pdfScene::save() {
  saveToFile( myFileName );
  /*
  QTemporaryFile *tmpSave = new QTemporaryFile( QDir::tempPath()+"/tempSaveXXXXXX" );
  tmpSave->setAutoRemove(false);
  saveToFile( tmpSave->fileName() );
  PoDoFo::PdfMemDocument pdfDoc;
  try { 
    pdfDoc.Load( tmpSave->fileName() );
  } catch ( PoDoFo::PdfError error ) { 
    qCritical() << "Cannot load the saved file:" << error.what();
    qCritical() << "Saving backup to :" << myFileName+"~backup";
    QFile::copy( tmpSave->fileName(), myFileName+"~backup" );
    return false;
  }
  if ( pdfDoc.GetPageCount() != numPages ) { 
    qCritical() << "Page count mismatch when saving file"
    qCritical() << "Saving backup to :" << myFileName+"~backup";
    QFile::copy( tmpSave->fileName(), myFileName+"~backup" );
    return false;
  }*/
}

void pdfScene::placeAnnotation( abstractAnnotation *annot, const QPointF *scPos ) { 
  int pg = posToPage( *scPos );
  QGraphicsItem *parentPage = getPageItem( pg );
  annot->setPos( parentPage->mapFromScene( *scPos ) );
  annot->setParentItem( parentPage );
  qDebug() << "Placing Annotation at page " << pg << " relative position " << parentPage->mapFromScene( *scPos ) << "=="<<annot->pos()<< " absolute position " << *scPos;
}

pdfPageItem *pdfScene::getPageItem( int pgNum ) { 
  QPointF topLeft = topLeftPage( pgNum );
  pdfPageItem *pg;
  foreach( QGraphicsItem *item, items( topLeft ) ) { 
    if ( pg = dynamic_cast<pdfPageItem*>( item ) ) { 
      if ( pg->getPageNum() == pgNum ) return pg;
    }
  }
  return NULL;
}



int pdfScene::posToPage( const QPointF &scenePos ) { 
  qreal pos = scenePos.y();
  int min = 0, max = pageCorners.size()-1, pivot=min+(max-min)/2,dist;
  if ( max < min ) return min;
  qreal pivotVal;
  while( min < max ) { 
    dist=(max-min);
    pivotVal = pageCorners[pivot].y();
    if ( pos < pivotVal ) max = pivot;
    else if (pivotVal < pos ) min = pivot;
    else return pivot;
    pivot = min+(max-min)/2;
    if ( (max-min) >=dist ) {
      if ( pageCorners[max].y() < pos ) return max;
      return min;
    }
  }
  return pivot;
};


QPointF pdfScene::topLeftPage( int page ) {
  if ( page < pageCorners.size() ) return pageCorners[page];
  return QPointF(0,0);
}

sceneLayer *pdfScene::addLayer() { 
  sceneLayer *ret = new sceneLayer( this );
  sceneLayers.append( ret );
  return ret;
}

void pdfScene::removeLayer( sceneLayer *layer ) { 
  sceneLayers.removeAll( layer );
  delete layer;
}

QList<TextBox *> pdfScene::selectText( QPointF from, QPointF to ) { 
  int pg = posToPage( from );
  Q_ASSERT( pg < textLayer.size() );
  QPointF fromP, toP;
  pdfPageItem *Page = getPageItem( pg );
  fromP = Page->mapFromScene( from );
  toP = Page->mapFromScene( to );
  return textLayer[pg]->select( fromP, toP );
}

QList< pageSelections > pdfScene::findText( QString text, int startPage, int endPage ) { 
  int totalNumOfMatches = 0;
  if ( endPage == -1 || endPage >= numPages ) endPage = numPages;
  if ( startPage < 0 ) startPage = 0;
  QList< pageSelections > ret;
  pageSelections sel;
  ret.clear();
  for( int i = startPage; i < endPage; ++i ) { 
    sel.pageNum = i;
    sel.selections=textLayer[i]->findText( text );
    if ( sel.selections.size() > 0 ) {
      ret.append( sel );
      totalNumOfMatches += sel.selections.size();
    }
  }
  return ret;
}

QString pdfScene::selectedText( QPointF from, QPointF to ) { 
  QList<TextBox*> selection = selectText( from, to );
  QString ret;
  foreach( TextBox *box, selection ) ret+=box->text()+" ";
  return ret;
}


