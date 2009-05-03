#include "linkTool.h"

#include <QtGui/QTextEdit>
#include <QtGui/QStackedWidget>

#include <QtCore/QDebug>

#include <podofo/podofo.h>

#include "toolBox.h"
#include "linkTool.h"
#include "pdfScene.h"
#include "pdfUtil.h"

QPixmap *linkTool::icon = NULL;
int linkTool::nameCounter = 0;

bool linkTool::acceptEventsFor( QGraphicsItem *item ) {
 if ( dynamic_cast<linkAnnotation*>(item) ) {
    return true;
  }
  return false;
}

linkTool::linkTool( pdfScene *Scene, toolBox *ToolBar, QStackedWidget *EditArea):
        abstractTool( Scene, ToolBar, EditArea )
{
  setToolName( "Link Tool" );

  if ( ! icon ) icon = new QPixmap( "link.png" );
  toolBar->addTool( QIcon(*icon), this );
}

QString linkTool::addDestination( PoDoFo::PdfDestination &dest ) {
    struct destination tgt;
    pdfCoords transform( dest.GetPage() );
    tgt.name = QString::number( nameCounter++ );
    tgt.page = dest.GetPage()->GetPageNumber();
    tgt.type = dest.GetType();
    if ( tgt.type == "XYZ" ) {
        PoDoFo::PdfRect rect( dest.GetLeft(), dest.GetTop(), 0, 0 );
        tgt.viewPort = transform->pdfToScene( & rect );
    } else if ( tgt.type == "FitR" ) {
        PoDoFo::PdfRect rect = dest.GetRect();
        tgt.viewPort = transform->pdfRectToScene( & rect );
    } else {
        tgt.viewPort = QRectF(0,0,0,0);
        tgt.value = dest.GetValue();
    }
    targets.push_back( tgt );
    return tggt.name;
}

QString linkTool::addDestination( int page, QRectF viewPort, QString name ) {
    struct destination tgt;
    if ( name == "" ) {
        tgt.name = QString::number( nameCounter++ );
    } else tgt.name = name;
    tgt.page = page;
    tgt.type="FitR";
    tgt.viewPort = viewPort;
    return tgt.name();
}


abstractAnnotation *linkTool::processAnnotation( PoDoFo::PdfAnnotation *annotation, pdfCoords *transform ) {
  if ( ! linkAnnotation::isA( annotation ) ) return NULL;
  return new linkAnnotation( this, annotation, transform );
}

void linkTool::newActionEvent( const QPointF *ScenePos ) {
  qDebug() << "Creating new Linl Annotation at " << *ScenePos;
  linkAnnotation *annot = new linkAnnotation( this );
  annot->setZValue( 10 );
  scene->placeAnnotation( annot, ScenePos );
  editItem( annot );
}



linkAnnotation::linkAnnotation( linkTool *tool, PoDoFo::PdfAnnotation *Link, pdfCoords *transform ):
        abstractAnnotation( tool, Link, transform )
{
  setIcon( linkTool::icon->scaledToHeight(20) );
  if ( Link->HasDestination() ) {
      tool->addDestination( Link->GetDestination() );
  }
  setZValue( 10 );
}


bool linkAnnotation::isA( PoDoFo::PdfAnnotation *annotation ) {
  if ( ! annotation ) return false;
  return ( annotation->GetType() == PoDoFo::ePdfAnnotation_Link );
}


void linkAnnotation::saveToPdfPage( PoDoFo::PdfDocument *document, PoDoFo::PdfPage *pg, pdfCoords *coords ) {
  qDebug() << "Saving annotation for "<<getAuthor() <<" : " << pos();
  QRectF bbox = mapToParent(boundingRect()).boundingRect();
  PoDoFo::PdfRect *brect = coords->sceneToPdf( bbox );
  PoDoFo::PdfAnnotation *annot = pg->CreateAnnotation( PoDoFo::ePdfAnnotation_Text, *brect );
  saveInfo2PDF( annot );
  delete brect;
}




#include "linkTool.moc"
