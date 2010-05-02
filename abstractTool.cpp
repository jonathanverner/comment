/**  This file is part of project comment
 *
 *  File: abstractTool.cpp
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



#include "abstractTool.h"
#include "myToolTip.h"
#include "pageView.h"
#include "pdfScene.h"
#include "toolBox.h"
#include "pdfUtil.h"
#include "renderTeX.h"
#include "propertyTab.h"
#include "hiliteItem.h"

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
#include <QtGui/QClipboard>
#include <QtGui/QApplication>

#include <QtCore/QDebug>

#include <podofo/podofo.h>

void abstractTool::nextEditorTab() { 
  int cur_pos = editor->currentIndex(), max = editor->count();
  cur_pos++;
  if ( cur_pos >= max ) cur_pos = 0;
  editor->setCurrentIndex( cur_pos );
}

void abstractTool::prevEditorTab() { 
  int cur_pos = editor->currentIndex(), max = editor->count();
  cur_pos--;
  if ( cur_pos < 0 ) cur_pos = max-1;
  editor->setCurrentIndex( cur_pos );
}


abstractTool::abstractTool( pdfScene *Scene, toolBox *ToolBar, QStackedWidget *EditArea ):
	editArea(EditArea), scene(Scene), toolBar(ToolBar), currentEditItem( NULL ) {
	  cntxMenu = new QMenu();
	  hi = new hiliteItem();
	  hi->setColor( QColor(0,0,0,100) );
	  hi->setZValue( 40 );
	  scene->addItem(hi);
	
	  contentEdit = new QTextEdit( EditArea );
	  propertyEdit = new propertyTab( EditArea );

	  editor = new QTabWidget( EditArea );
	  editor->addTab( contentEdit, QString( "Content" ) );
	  editor->addTab( propertyEdit, QString( "Properties" ) );
	  QAction *rightTab = new QAction( editor ), *leftTab = new QAction( editor );
	  QAction *closeEdit = new QAction( editor );
	  rightTab->setShortcut((QString) "Ctrl+.");
	  leftTab->setShortcut((QString) "Ctrl+,");
	  closeEdit->setShortcut((QString) "Esc");
	  editor->addAction( rightTab );
	  editor->addAction( leftTab );
	  editor->addAction( closeEdit );
	  connect( rightTab, SIGNAL( triggered() ), this, SLOT( nextEditorTab() ) );
	  connect( leftTab, SIGNAL( triggered() ), this, SLOT( prevEditorTab() ) );
	  connect( closeEdit, SIGNAL( triggered() ), this, SLOT( hideEditor() ) );
	  connect( propertyEdit, SIGNAL( authorChanged() ), this, SLOT( updateAuthor() ) );  	
	  connect( contentEdit, SIGNAL( textChanged() ), this, SLOT( updateContent() ) );
	  editArea->addWidget( editor );
	  renderer = new renderTeX;

	  QAction *delAct  = cntxMenu->addAction( "Delete" );
	  QAction *proAct  = cntxMenu->addAction( "Properties...");
	  connect( delAct, SIGNAL( triggered() ), this, SLOT( deleteCurrentAnnotation() ) );
	  connect( proAct, SIGNAL( triggered() ), this, SLOT( editCurrentAnnotationProperties() ) );
	  connect( renderer, SIGNAL( itemReady(int) ), this, SLOT( teXToolTipReady(int) ) );
	}

abstractTool::~abstractTool() {
  editArea->removeWidget( editor );
  toolBar->removeTool( this );
  if ( editor ) delete editor;
  delete hi;
}

void abstractTool::updateContent() {
  if ( ! currentEditItem ) return;
  currentEditItem->setContent( contentEdit->toPlainText() );
}

void abstractTool::updateAuthor() {
  if ( ! currentEditItem ) return;
  qDebug() << "updating Author to ..." << propertyEdit->getAuthor() << "\n";
  currentEditItem->setAuthor( propertyEdit->getAuthor() );
}



void abstractAnnotation::setContent( QString Content ) { 
  content = Content;
  setMyToolTip( content );
}


int abstractTool::getApproxWidth( QString text ) { 
  QLabel label( text );
  qreal mm =  (label.width()/72)*10;
  return qRound(mm);
}



void abstractTool::setTeXToolTip( abstractAnnotation *annot ) { 
  QString content = annot->getContent();
  if ( content.contains( "$" ) ) { 
    int id = renderer->addItem( content );
    if ( id < int2annot.size() ) int2annot[id] = annot;
    else { 
      int2annot.resize( id + 10 );
      int2annot[id] = annot;
    }
    renderer->preRender( id, false, getApproxWidth( content ) );
  }
}

void abstractTool::teXToolTipReady( int annotID ) { 
  int2annot[annotID]->setMyToolTip( renderer->render( annotID, false, 1.5 ) );
  renderer->deleteItem( annotID );
}

void abstractTool::deleteCurrentAnnotation() { 
  if ( currentEditItem ) {
    scene->removeItem( currentEditItem );
    delete currentEditItem;
    if ( editArea->isVisible() ) editArea->hide();
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
    setTeXToolTip( item );
  } else {
    currentEditItem = item;
    editArea->setCurrentWidget( editor );
    editArea->show();
    propertyEdit->setAuthor( item->getAuthor() );
    contentEdit->setText( item->getContent() );
    contentEdit->setFocus();
    editor->setCurrentIndex( 0 );
  }
}

bool abstractTool::handleEvent( viewEvent *ev ) { 
  if ( ev->type() == viewEvent::VE_MOUSE_RELEASE && ( ev->btnCaused() == Qt::LeftButton ) ) { 
    QPointF pos=ev->scenePos(), delta = ev->sceneDelta();
    if ( ev->isClick() ) {
      qDebug() << "Single click...";
      abstractAnnotation *annot = ev->topItem();
      if ( annot ) editItem( annot );
      else newActionEvent( &pos );
      return true;
    }
  } if ( ev->type() == viewEvent::VE_MOUSE_MOVE && currentEditItem ) {
      editItem( currentEditItem );
      return true;
  } if ( ev->type() == viewEvent::VE_MOUSE_PRESS && ( ev->btnCaused() == Qt::RightButton ) ) { 
    hi->clear();
    hi->setPos( scene->topLeftPage(scene->posToPage( ev->scenePos() ) ) );
    hi->show();
    selectedText = "";
  } else if ( ev->type() == viewEvent::VE_MOUSE_RELEASE && (ev->btnCaused() == Qt::RightButton ) ) {
    qDebug() << "Selected: " << selectedText;
    QApplication::clipboard()->setText( selectedText, QClipboard::Selection );
  } else if ( ev->type() == viewEvent::VE_MOUSE_MOVE && (ev->btnState() & Qt::RightButton ) ) { 
    hi->updateBBoxes( scene->selectText( ev->mousePressPos(), ev->scenePos() ) );
    selectedText = scene->selectedText( ev->mousePressPos(), ev->scenePos() );
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
  connect( this, SIGNAL(needKeyFocus(bool)), tool, SIGNAL(needKeyFocus(bool)) );
}

abstractAnnotation::abstractAnnotation( abstractTool *tool, PoDoFo::PdfAnnotation *annot, pdfCoords *transform ):
	myTool( tool ), haveToolTip( false ), showingToolTip( false ), movable( true )
{ 
  setAcceptsHoverEvents( true );
  if ( annot ) { 
    setAuthor( pdfUtil::pdfStringToQ( annot->GetTitle() ) );
    setContent( pdfUtil::pdfStringToQ( annot->GetContents() ) );
    PoDoFo::PdfRect ps = annot->GetRect();
    setPos( transform->pdfRectToScene( &ps ).topLeft() );
    tool->setTeXToolTip( this );
  } else { 
    setAuthor( tool->getAuthor() );
  }
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
    annot->SetFlags( 0 ); // unset all flags to allow everything
    annot->SetBorderStyle( 0, 0, 0 );
  } catch ( PoDoFo::PdfError error ) { 
    qWarning() << "Error setting annotation properties:" << error.what();
  }
}


#include "abstractTool.moc"







