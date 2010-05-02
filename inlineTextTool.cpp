/**  This file is part of project comment
 *
 *  File: inlineTextTool.cpp
 *  Created: 2008-11-07
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



#include "inlineTextTool.h"
#include "pageView.h" /* needed for viewEvent */
#include "toolBox.h"
#include "pdfScene.h"
#include "pdfUtil.h"
#include "propertyTab.h"
#include "renderTeX.h"

#include <QtCore/QDebug>
#include <QtGui/QIcon>
#include <QtGui/QStackedWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QTabWidget>
#include <QtGui/QStyleOption>

#include <poppler-qt4.h>

using namespace Poppler;

QIcon inlineTextTool::icon;

inlineTextTool::inlineTextTool( pdfScene *Scene, toolBox *ToolBar, QStackedWidget *EditArea):
	abstractTool( Scene, ToolBar, EditArea )
{
  icon = QIcon::fromTheme("draw-text");
  setToolName( "Inline Text Tool" );
  toolBar->addTool( QIcon(icon), this );
  inlineRenderer = new renderTeX();
  connect( inlineRenderer, SIGNAL(itemReady(int)), this, SLOT(teXReady(int)) );
}

inlineTextTool::~inlineTextTool() {
  delete inlineRenderer;
}



void inlineTextTool::newActionEvent( const QPointF *ScenePos ) {
  inlineTextAnnotation *an = new inlineTextAnnotation(this);
  if ( int2annot.size() <= an->inlineID ) int2annot.resize( an->inlineID + 10 );
  int2annot[an->inlineID] = an;
  scene->placeAnnotation( an, ScenePos );
  an->setZValue( 9 );
  qDebug() << "Editing inline text item";
  editItem( an );
}


bool inlineTextTool::acceptEventsFor( QGraphicsItem *item ) {  
  return dynamic_cast<inlineTextAnnotation*>( item );
}

void inlineTextTool::editItem( abstractAnnotation *item ) { 
  qDebug() << "inlineTextAnnotation::editItem";
  inlineTextAnnotation *ann = dynamic_cast<inlineTextAnnotation *>( item );
  if ( ! ann ) {
    qDebug() << "inlineTextAnnotation::editItem: NOT AN INLINE TEXT ANNOTATION !!!! WTF ?! ";
    return;
  }
  if ( currentEditItem == item ) finishEditing();
  else {
    currentEditItem = item;
    propertyEdit->setAuthor( item->getAuthor() );
    editor->setCurrentWidget( propertyEdit );
    emit needKeyFocus( true );
    ann->setTeXAppearance( false );
    ann->setMyToolTip( ann->getContent() );
    ann->item->setFocus();
  }
}


void inlineTextTool::finishEditing() {
  qDebug() << " Finish Editing ... ";
  inlineTextAnnotation *ann = dynamic_cast<inlineTextAnnotation*>(currentEditItem);
  if ( ann ) {
    ann->item->clearFocus();
    if ( ann->item->toPlainText() == "" ) deleteCurrentAnnotation();
    else {
      ann->setContent( ann->item->toPlainText() );
      ann->setTeXAppearance( false );
      prepareTeX( ann );
      editArea->hide();
      //setTeXToolTip( currentEditItem );
      currentEditItem = NULL;
    }
    emit needKeyFocus( false );
  }
}
	
void inlineTextTool::prepareTeX( inlineTextAnnotation *ann ) { 
  if ( ! ann->getContent().contains("$") ) return;
  qDebug() << " Request TeX rendering for item " << ann->inlineID << " ...";
  inlineRenderer->setItem( ann->inlineID, ann->getContent() );
  inlineRenderer->preRender( ann->inlineID );
}

void inlineTextTool::teXReady( int item ) {
  qDebug() << " Rendering finished for " << item;
  if ( (item < int2annot.size()) && (int2annot[item]) ) int2annot[item]->setTeXAppearance( true );
  else qDebug() << " Item does not exist anymore ... ";
}
  
/*bool inlineTextTool::handleEvent( viewEvent *ev ) { 
  inlineTextAnnotation *annot;
  if ( annot = dynamic_cast<inlineTextAnnotation*>(ev->item()) ) {
    return annot->sceneEvent( ev->getOriginalEvent() );
  } else {
    QPointF pos = ev->scenePos();
    newActionEvent( &pos );
  }
}*/


abstractAnnotation *inlineTextTool::processAnnotation( PoDoFo::PdfAnnotation *annotation, pdfCoords *transform ) {
  if ( ! inlineTextAnnotation::isA( annotation ) ) return NULL;
  inlineTextAnnotation *ann = new inlineTextAnnotation( this, annotation, transform );
  if ( int2annot.size() <= ann->inlineID ) int2annot.resize( ann->inlineID + 10 );
  int2annot[ann->inlineID] = ann;
  prepareTeX( ann );
  return ann;
}


void inlineTextAnnotation::setTeXAppearance( bool haveTeX ) {
  if ( haveTeX == teXAppearance ) return;
  prepareGeometryChange();
  teXAppearance=haveTeX;
  if ( haveTeX ) {
    inlineTextTool *tl = dynamic_cast<inlineTextTool*>(myTool);
    QRectF bBox = tl->inlineRenderer->getBBox( inlineID );
    brec = QRectF(0,0,bBox.width(), bBox.height());
    update();
    item->hide();
  }
  else item->show();
}

void inlineTextAnnotation::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget ) {  
  if ( teXAppearance ) {
#if QT_VERSION < 0x040600
    qreal zoom = option->levelOfDetail;
#else
    qreal zoom = option->levelOfDetailFromTransform( painter->worldTransform() );
#endif
    inlineTextTool *tl = dynamic_cast<inlineTextTool*>(myTool);
    QPixmap pix = tl->inlineRenderer->render( inlineID, false, zoom );
    //brec = QRectF(0,0,(qreal) pix.width()/(qreal)zoom, (qreal)pix.height()/(qreal)zoom);
    QRectF exposed = option->exposedRect;
    qreal x,y,w,h;
    option->exposedRect.getRect( &x, &y, &w, &h );
    qDebug() << "Painting pixmap";
    painter->drawPixmap( exposed, pix, QRectF( x*zoom,y*zoom,w*zoom,h*zoom ) );
  } else item->paint( painter, option, widget );
  //painter->drawRect( item->boundingRect() );
}

QRectF inlineTextAnnotation::boundingRect() const {
  if ( teXAppearance ) {
    return brec;
  }
  else return item->boundingRect();
}

QPainterPath inlineTextAnnotation::shape() const {
  if ( teXAppearance ) { 
    QPainterPath path;
    path.addRect( brec );
    return path;
  } else return item->shape();
}


int inlineTextAnnotation::inlineAnnotationCount = 0;

inlineTextAnnotation::inlineTextAnnotation( inlineTextTool *tool, PoDoFo::PdfAnnotation *inlineAnnot, pdfCoords *transform): 
	abstractAnnotation(tool, inlineAnnot, transform), teXAppearance(false)
{
  inlineID = inlineAnnotationCount++;
  item = new QGraphicsTextItem();
  item->setTextInteractionFlags( Qt::TextEditable );
  item->setParentItem(this);
  item->setPos(0,0);
  item->setZValue( 8 );
  item->setPlainText(getContent());
  movable=true;
  setZValue( 9 );
};

inlineTextAnnotation::~inlineTextAnnotation() {
  delete item;
  inlineTextTool *tl = dynamic_cast<inlineTextTool*>(myTool);
  tl->int2annot[inlineID]=NULL;
}

bool inlineTextAnnotation::isA( PoDoFo::PdfAnnotation *annotation ) { 
  if ( ! annotation ) return false;
    return ( annotation->GetType() == PoDoFo::ePdfAnnotation_FreeText );
}
void inlineTextAnnotation::saveToPdfPage( PoDoFo::PdfDocument *document, PoDoFo::PdfPage *pg, pdfCoords *coords ) {
  inlineTextTool *tl = dynamic_cast<inlineTextTool*>(myTool);
  qDebug() << "Saving INLINE annotation for "<<getAuthor()<<" : " << pos() << " BBox:" << mapToParent(boundingRect()).boundingRect();
  PoDoFo::PdfRect *brect = coords->sceneToPdf( mapToParent(boundingRect()).boundingRect() );
  qDebug() << "PDF Rect: (" << brect->GetLeft() << ", "<< brect->GetBottom() << " " << brect->GetWidth() << "x"<<brect->GetHeight() << ")";
  //brect->SetHeight(boundingRect().height());
  //brect->SetWidth(boundingRect().width());
  PoDoFo::PdfAnnotation *annot = pg->CreateAnnotation( PoDoFo::ePdfAnnotation_FreeText, *brect );
  if ( teXAppearance ) {
    QByteArray path = tl->inlineRenderer->getPDF( inlineID ).toUtf8();
    QRectF cropBox = tl->inlineRenderer->getBBox( inlineID );
    qDebug() << "Appearance BBox (in QT): " << cropBox;
    PoDoFo::PdfMemDocument pdfDoc( path.data() );
    PoDoFo::PdfRect pgSz = pdfDoc.GetPage(0)->GetPageSize();
    PoDoFo::PdfObject trimbox;
    PoDoFo::PdfRect trim( cropBox.x(), cropBox.y(), cropBox.width(), cropBox.height() );
    trim.ToVariant(trimbox);
    PoDoFo::PdfXObject *annotAppearance = new PoDoFo::PdfXObject( pdfDoc, 0, document );
    annotAppearance->GetContentsForAppending()->GetDictionary().AddKey( PoDoFo::PdfName("BBox"), trimbox );
//     PoDoFo::PdfDictionary dict,privDict;
//     try {
//     privDict.AddKey( "On", annotAppearance->GetObject()->Reference());
//     dict.AddKey( "N", privDict );
//     } catch (PoDoFo::PdfError e) { 
//       qDebug() <<"dict.AddKey(N,ap_ref) failed: "<<e.what();
//     }
//     try {
//     annot->GetObject()->GetDictionary().AddKey( "AP", dict);
//     annot->GetObject()->GetDictionary().AddKey( "AS", PoDoFo::PdfName("On") );
//     } catch (PoDoFo::PdfError e) { 
//       qDebug() <<"annot.AddKey(AP,dict) failed: "<<e.what();
//     }
    annot->SetAppearanceStream( annotAppearance );
    delete annotAppearance;
  }
  saveInfo2PDF( annot );
  delete brect;
}



#include "inlineTextTool.moc"