/***************************************************************
 * textTool.cpp
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


#include <QtGui/QTextEdit>
#include <QtGui/QStackedWidget>

#include <QtCore/QDebug>

#include <podofo/podofo.h>

#include "toolBox.h"
#include "textTool.h"
#include "pdfScene.h"
#include "pdfUtil.h"

QPixmap *textTool::icon = NULL;

bool textTool::acceptEventsFor( QGraphicsItem *item ) {
  if ( dynamic_cast<textAnnotation*>(item) ) {
    return true;
  }
  return false;
}

textTool::textTool( pdfScene *Scene, toolBox *ToolBar, QStackedWidget *EditArea):
	abstractTool( Scene, ToolBar, EditArea )
{
  setToolName( "Text Tool" );

  if ( ! icon ) icon = new QPixmap( "comment.png" );
  toolBar->addTool( QIcon(*icon), this );
}

abstractAnnotation *textTool::processAnnotation( PoDoFo::PdfAnnotation *annotation, pdfCoords *transform ) {
  if ( ! textAnnotation::isA( annotation ) ) return NULL;
  return new textAnnotation( this, annotation, transform );
}

void textTool::newActionEvent( const QPointF *ScenePos ) {
  qDebug() << "Creating new text Annotation at " << *ScenePos;
  textAnnotation *annot = new textAnnotation( this );
  annot->setZValue( 10 );
  scene->placeAnnotation( annot, ScenePos );
  editItem( annot );
}



textAnnotation::textAnnotation( textTool *tool, PoDoFo::PdfAnnotation *Comment, pdfCoords *transform ):
	abstractAnnotation( tool, Comment, transform )
{
  setIcon( textTool::icon->scaledToHeight(20) );
  setZValue( 10 );
}


bool textAnnotation::isA( PoDoFo::PdfAnnotation *annotation ) {
  if ( ! annotation ) return false;
  return ( annotation->GetType() == PoDoFo::ePdfAnnotation_Text );
}


void textAnnotation::saveToPdfPage( PoDoFo::PdfDocument *document, PoDoFo::PdfPage *pg, pdfCoords *coords ) { 
  qDebug() << "Saving text annotation for "<<getAuthor() <<" : " << pos();
  QRectF bbox = mapToParent(boundingRect()).boundingRect();
  PoDoFo::PdfRect *brect = coords->sceneToPdf( bbox );
  PoDoFo::PdfAnnotation *annot = pg->CreateAnnotation( PoDoFo::ePdfAnnotation_Text, *brect );
  saveInfo2PDF( annot );
  delete brect;
}




#include "textTool.moc"
