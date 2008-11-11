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

void textTool::editItem( abstractAnnotation *item ) { 
  abstractTool::editItem( item );
  QTextEdit *edt = dynamic_cast<QTextEdit*>(editor);
  edt->setText( dynamic_cast<textAnnotation*>(item)->getText() );
  edt->setFocus();
}



textTool::textTool( pdfScene *Scene, toolBox *ToolBar, QStackedWidget *EditArea):
	abstractTool( Scene, ToolBar, EditArea )
{
  setToolName( "Text Tool" );

  if ( ! icon ) icon = new QPixmap( "comment.png" );
  QTextEdit *edt = new QTextEdit( EditArea );
  editor = edt;
  connect( edt, SIGNAL( textChanged() ), this, SLOT( updateComment() ) );
  editArea->addWidget( editor );
  toolBar->addTool( QIcon(*icon), this );
}

textTool::~textTool() {
  editArea->removeWidget( editor );
  toolBar->removeTool( this );
  delete editor;
}

abstractAnnotation *textTool::processAnnotation( PoDoFo::PdfAnnotation *annotation, pdfCoords *transform ) {
  if ( ! textAnnotation::isA( annotation ) ) return NULL;
  return new textAnnotation( this, annotation, transform );
}

void textTool::newActionEvent( const QPointF *ScenePos ) {
  qDebug() << "Creating new Annotation at " << *ScenePos;
  textAnnotation *annot = new textAnnotation( this );
  annot->setAuthor( author );
  annot->setZValue( 10 );
  scene->placeAnnotation( annot, ScenePos );
  editItem( annot );
}

void textTool::updateComment() {
  if ( ! currentEditItem ) return;
  textAnnotation *annot = dynamic_cast<textAnnotation*>(currentEditItem);
  annot->setText( dynamic_cast<QTextEdit*>(editor)->toPlainText() );
}


textAnnotation::textAnnotation( textTool *tool, PoDoFo::PdfAnnotation *Comment, pdfCoords *transform ):
	abstractAnnotation( tool )
{
  setIcon( textTool::icon->scaledToHeight(20) );
  if ( isA( Comment ) ) {
    setAuthor( pdfUtil::pdfStringToQ( Comment->GetTitle() ) );
    setText( pdfUtil::pdfStringToQ( Comment->GetContents() ) );
    PoDoFo::PdfRect pos = Comment->GetRect();
    setPos( transform->pdfToScene( &pos ) );
  }
    setZValue( 10 );
}

textAnnotation::~textAnnotation() { 
};

void textAnnotation::setText(QString Comment) {
  comment = Comment;
  setMyToolTip( comment );
}

bool textAnnotation::isA( PoDoFo::PdfAnnotation *annotation ) {
  if ( ! annotation ) return false;
  return ( annotation->GetType() == PoDoFo::ePdfAnnotation_Text );
}


void textAnnotation::saveToPdfPage( PoDoFo::PdfDocument *document, PoDoFo::PdfPage *pg, pdfCoords *coords ) { 
  qDebug() << "Saving annotation: " << pos();
  PoDoFo::PdfRect *brect = coords->sceneToPdf( pos() );
  PoDoFo::PdfAnnotation *annot = pg->CreateAnnotation( PoDoFo::ePdfAnnotation_Text, *brect );
  try { 
    annot->SetOpen( false );
    annot->SetContents( pdfUtil::qStringToPdf( comment ) );
    annot->SetTitle( pdfUtil::qStringToPdf( getAuthor() ) );
  } catch ( PoDoFo::PdfError error ) { 
    qDebug() << "Error setting annotation properties:" << error.what();
  }
  delete brect;
}




#include "textTool.moc"
