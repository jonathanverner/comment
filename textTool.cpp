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

QPixmap *textTool::icon = NULL;

bool textTool::acceptEventsFor( QGraphicsItem *item ) { 
  if ( dynamic_cast<textAnnotation*>(item) ) return true;
  return false;
}

void textTool::editItem( abstractAnnotation *item ) { 
  qDebug()<<"Editing... textAnnotation";
  currentEditItem = item;
  QTextEdit *edt = dynamic_cast<QTextEdit*>(editor);
  editArea->setCurrentWidget( editor );
  editArea->show();
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
  textAnnotation *annot = new textAnnotation( this );
  scene->addItem( annot );
  annot->setPos( *ScenePos );
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
  setIcon( *textTool::icon );
  if ( isA( Comment ) ) { 
    std::string content = Comment->GetContents().GetStringUtf8();
    std::string author = Comment->GetTitle().GetStringUtf8();
    setAuthor( QString::fromUtf8( author.c_str() ) );
    setText( QString::fromUtf8( content.c_str() ) );
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

//void textAnnotation::saveToPdfPage( PoDoFo:PdfDocument *document, int page ) {
//  QPointF pagePos = scene()->globalToPagePos( scenePos() );
//  PoDoFo::PdfPage *pdfPage = document->GetPage( page );
//  PoDoFo::PdfAnnotation *pdfAnnotation = pdfPage->CreateAnnotation( ePdfAnnotation_Text, PdfRect )
//}


#include "textTool.moc"
