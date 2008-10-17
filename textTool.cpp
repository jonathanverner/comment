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

#include "toolBox.h"
#include "textTool.h"
#include "pdfScene.h"

QPixmap comment::Tool *icon = NULL;

textTool::textTool( pdfScene *Scene, QToolBar *ToolBar, QStackedWidget *EditArea):
	abstractTool( Scene, ToolBar, EditArea )
{
  if ( ! icon ) icon = new QPixmap( "comment.png" );
  QTextEdit edt = new QTextEdit(EditArea);
  editor = edt;
  connect( edt, SIGNAL( textChanged() ), this, SLOT( updateComment() ) );
  editArea->addWidget( editor );
  toolBar->addTool( icon, this, SLOT( newActionEvent(QPoint*) ), SLOT( editItem(abstractAnnotation*) ) );
}

textTool::~textTool() {
  editArea->removeWidget( editor );
  toolBar->removeTool( this );
  delete editor;

}

bool textTool::processAnnotation( PoDoFo::PdfAnnotation *annotation, int page ) {
  if ( ! textAnnotation::isA( annotation ) ) return false;
  textAnnotation *annot = new textAnnotation( this, page, annotation );
  scene->add( annot );

  /* needs some more thought, for the moment
   * set it to 10 so that it is at least above
   * the pageItems */
  annot->setZValue( 10 );
  return true;
}

void textTool::newActionEvent( QPoint *ScenePos ) {
  textAnnotation *annot = new textAnnotation( this, scene->posToPage( *ScenePos ) );
  scene->add( annot );
  annot->setPos( *ScenePos );
  editItem( annot );
}

void textTool::updateComment() {
  if ( ! currentEditItem ) return;
  textAnnotation *annot = dynamic_cast<textAnnotation*>(currentItem);
  annot->setText( dynamic_cast<QTextEdit*>(editor)->toHtml() );
}

textAnnotation::textAnnotation( textTool *tool, int page, PoDoFo::PdfAnnotation *Comment):
	abstractAnnotation( tool, page, Comment );
{
  if ( isA( Comment ) ) { 
    std::string content = Comment->GetContents().GetStringUtf8();
    std::string author = Comment->GetTitle().GetStringUtf8();
    setAuthor( QString::fromUtf8( author.c_str() ) );
    setText( QString::fromUtf8( content.c_str() ) );
    PoDoFo::PdfRect pos = Comment->GetRect();
    setPos( pos.GetLeft(), pos.GetBottom()+pos.GetHeight() );
  }
}


void textAnnotation::setText(QString Comment) {
  comment = Comment;
}

bool textAnnotation::isA( PoDoFo::PdfAnnotation *annotation ) {
  if ( ! annotation ) return false;
  return ( annotation->GetType() == ePdfAnnotation_Text );
}


#include "textTool.moc"
