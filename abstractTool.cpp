/***************************************************************
 * abstractTool.cpp
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
#include <QtGui/QStackedWidget>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QGraphicsSceneHoverEvent>
#include <QtGui/QPainter>
#include <QtGui/QStyleOptionGraphicsItem>
#include <QtGui/QLabel>
#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtGui/QTextEdit>
#include <QtGui/QTabWidget>

#include <QtCore/QDebug>

#include <podofo/podofo.h>

#include "abstractTool.h"
#include "myToolTip.h"
#include "pageView.h"
#include "pdfScene.h"
#include "toolBox.h"
#include "pdfUtil.h"


abstractTool::abstractTool( pdfScene *Scene, toolBox *ToolBar, QStackedWidget *EditArea ):
	editArea(EditArea), scene(Scene), toolBar(ToolBar), currentEditItem( NULL ) {
	  cntxMenu = new QMenu();
	
	  contentEdit = new QTextEdit( EditArea );
	  editor = new QTabWidget( EditArea );
	  editor->addTab( contentEdit, QString( "Content" ) );
  	
	  connect( contentEdit, SIGNAL( textChanged() ), this, SLOT( updateContent() ) );
	  editArea->addWidget( editor );

	  QAction *delAct  = cntxMenu->addAction( "Delete" );
	  QAction *proAct  = cntxMenu->addAction( "Properties...");
	  connect( delAct, SIGNAL( triggered() ), this, SLOT( deleteCurrentAnnotation() ) );
	  connect( proAct, SIGNAL( triggered() ), this, SLOT( editCurrentAnnotationProperties() ) );
	}

abstractTool::~abstractTool() {
  editArea->removeWidget( editor );
  toolBar->removeTool( this );
  if ( editor ) delete editor;
}

void abstractTool::updateContent() {
  if ( ! currentEditItem ) return;
  currentEditItem->setContent( contentEdit->toPlainText() );
}

void abstractAnnotation::setContent( QString Content ) { 
  content = Content;
  setMyToolTip( content );
}


void abstractTool::deleteCurrentAnnotation() { 
  if ( currentEditItem ) {
    scene->removeItem( currentEditItem );
    delete currentEditItem;
  }
  currentEditItem = NULL;
}

void abstractTool::hideEditor() { 
  if ( editArea ) editArea->hide();
}


void abstractTool::editCurrentAnnotationProperties() { 
  qDebug() << "Editing properties...";
}

QMenu *abstractTool::contextMenu( QGraphicsItem *it ) { 
  currentEditItem = dynamic_cast<abstractAnnotation*>(it);
  return cntxMenu;
}

/* Called by an item, which wants to be edited. The item passes
 * * a reference to itself */
void abstractTool::editItem( abstractAnnotation *item ) {
  if ( currentEditItem == item ) { 
    currentEditItem = NULL;
    editArea->hide();
  } else {
    currentEditItem = item;
    editArea->setCurrentWidget( editor );
    editArea->show();
    contentEdit->setText( item->getContent() );
    contentEdit->setFocus();
    editor->setCurrentIndex( 0 );
  }
}

bool abstractTool::handleEvent( viewEvent *ev ) { 
  if ( ev->type() == viewEvent::VE_MOUSE_RELEASE && ( ev->btnCaused() == Qt::LeftButton ) ) { 
    QPointF pos=ev->scenePos(), delta = ev->sceneDelta();
    if ( ev->isClick() ) { 
      abstractAnnotation *annot = ev->topItem();
      if ( annot ) editItem( annot );
      else newActionEvent( &pos );
      return true;
    }
  } else return false;
}

QMenu *abstractAnnotation::contextMenu() { 
  return myTool->contextMenu( this );
}

abstractAnnotation::abstractAnnotation( abstractTool *tool ):
	myTool( tool ), date( QDate::currentDate() ), time( QTime::currentTime() ), haveToolTip(false), showingToolTip(false), movable( true )
{
  setAcceptsHoverEvents( true );
  setAuthor( tool->getAuthor() );
}

abstractAnnotation::abstractAnnotation( abstractTool *tool, PoDoFo::PdfAnnotation *annot, pdfCoords *transform ):
	myTool( tool ), haveToolTip( false ), showingToolTip( false ), movable( true )
{ 
  setAcceptsHoverEvents( true );
  setAuthor( pdfUtil::pdfStringToQ( annot->GetTitle() ) );
  setContent( pdfUtil::pdfStringToQ( annot->GetContents() ) );
  PoDoFo::PdfRect ps = annot->GetRect();
  setPos( transform->pdfToScene( &ps ) );
}


void abstractAnnotation::setMyToolTip(const QPixmap &pixMap) {
  setAcceptsHoverEvents(true);
  haveToolTip = true;
  tp = pixmap;
  toolTipPixMap = pixMap;
  if ( showingToolTip ) myToolTip::update( pixMap );
}

void abstractAnnotation::setMyToolTip(const QString &richText) { 
  setAcceptsHoverEvents(true);
  haveToolTip = true;
  tp = text;
  toolTipRichText = richText;
  if ( showingToolTip ) myToolTip::update( richText );
}



bool abstractAnnotation::editSelf() {
  myTool->editItem( this );
  return true;
}


bool abstractAnnotation::hasToolTip() { 
  if ( haveToolTip ) { 
    if ( tp == text && toolTipRichText.size() < 1 ) return false;
    return true;
  }
  return false;
}

bool abstractAnnotation::showToolTip( const QPoint &scPos ) { 
  qDebug() << "Showing Tooltip";
  if ( haveToolTip ) { 
    switch( tp ) { 
	    case pixmap:
		    myToolTip::showPixmap(scPos, toolTipPixMap );
		    showingToolTip = true;
		    return true;
		    break;
	    case text:
		    if ( toolTipRichText.size() < 1 ) return false;
		    myToolTip::showText(scPos, toolTipRichText );
		    showingToolTip = true;
		    return true;
		    break;
    }
  }
  return false;
}

void abstractAnnotation::hideToolTip() { 
  qDebug() << "Hiding Tooltip";
  myToolTip::hide();
  showingToolTip = false;
  myTool->editArea->hide();
}

void abstractAnnotation::setIcon(const QPixmap &icn) {
  if ( ! ( icn.rect() == icon.rect() ) ) prepareGeometryChange();
  icon = icn;
  update( icon.rect() );
}

QRectF abstractAnnotation::boundingRect() const {
  return (QRectF) icon.rect();
}

void abstractAnnotation::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget ) {
  painter->drawPixmap( option->exposedRect, icon, option->exposedRect );
}

void abstractAnnotation::saveInfo2PDF( PoDoFo::PdfAnnotation *annot ) { 
  try { 
    annot->SetOpen( false );
    annot->SetContents( pdfUtil::qStringToPdf( getContent() ) );
    annot->SetTitle( pdfUtil::qStringToPdf( getAuthor() ) );
  } catch ( PoDoFo::PdfError error ) { 
    qWarning() << "Error setting annotation properties:" << error.what();
  }
}


#include "abstractTool.moc"







