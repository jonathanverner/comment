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

textTool::textTool( pdfScene *Scene, toolBox *ToolBar, QStackedWidget *EditArea):
	abstractTool( Scene, ToolBar, EditArea )
{
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

abstractAnnotation *textTool::processAnnotation( PoDoFo::PdfAnnotation *annotation ) {
  if ( ! textAnnotation::isA( annotation ) ) return NULL;
  return new textAnnotation( this, annotation );
}

void textTool::newActionEvent( QPoint *ScenePos ) {
  textAnnotation *annot = new textAnnotation( this );
  scene->addItem( annot );
  annot->setPos( *ScenePos );
  editItem( annot );
}

void textTool::updateComment() {
  if ( ! currentEditItem ) return;
  textAnnotation *annot = dynamic_cast<textAnnotation*>(currentEditItem);
  annot->setText( dynamic_cast<QTextEdit*>(editor)->toHtml() );
}


textAnnotation::textAnnotation( textTool *tool, PoDoFo::PdfAnnotation *Comment):
	abstractAnnotation( tool )
{
  setIcon( *textTool::icon );
  if ( isA( Comment ) ) { 
    std::string content = Comment->GetContents().GetStringUtf8();
    std::string author = Comment->GetTitle().GetStringUtf8();
    setAuthor( QString::fromUtf8( author.c_str() ) );
    setText( QString::fromUtf8( content.c_str() ) );
    PoDoFo::PdfRect pos = Comment->GetRect();
    setPos( pos.GetLeft(), pos.GetBottom()+pos.GetHeight() );
    qDebug() << "Adding annotation: "<< QString::fromStdString(author) << QString::fromStdString(content);
  }
}

textAnnotation::~textAnnotation() { 
};

void textAnnotation::setText(QString Comment) {
  comment = Comment;
}

bool textAnnotation::isA( PoDoFo::PdfAnnotation *annotation ) {
  if ( ! annotation ) return false;
  return ( annotation->GetType() == PoDoFo::ePdfAnnotation_Text );
}

//void textAnnotation::saveToPdfPage( PoDoFo:PdfDocument *document, int page ) {
//}


#include "textTool.moc"
