/**  This file is part of project comment
 *
 *  File: textTool.cpp
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




#include "toolBox.h"
#include "textTool.h"
#include "pdfScene.h"
#include "pdfUtil.h"


#include <QtGui/QTextEdit>
#include <QtGui/QStackedWidget>

#include <QtCore/QDebug>

#include <podofo/podofo.h>

QIcon textTool::icon;

bool textTool::acceptEventsFor( QGraphicsItem *item ) {
  if ( dynamic_cast<textAnnotation*>(item) ) {
    return true;
  }
  return false;
}

textTool::textTool( pdfScene *Scene, toolBox *ToolBar, QStackedWidget *EditArea):
	abstractTool( Scene, ToolBar, EditArea )
{
  icon = QIcon::fromTheme("document-edit");
  setToolName( "Text Tool" );
  toolBar->addTool( icon, this );
}

abstractAnnotation *textTool::processAnnotation( PoDoFo::PdfDocument* doc, PoDoFo::PdfAnnotation* annotation, pdfCoords* transform ) {
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
  setIcon( QIcon::fromTheme("flag").pixmap(20) );
  setZValue( 10 );
}


bool textAnnotation::isA( PoDoFo::PdfAnnotation *annotation ) {
  if ( ! annotation ) return false;
  return ( annotation->GetType() == PoDoFo::ePdfAnnotation_Text );
}

#include "textTool.moc"
