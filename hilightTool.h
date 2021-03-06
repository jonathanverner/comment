#ifndef _hilightTool_H
#define _hilightTool_H

/**  This file is part of comment
*
*  File: hilightTool.h
*  Created: 7. 11. 2009
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


#include <QtCore/QList>
#include <QtGui/QIcon>
#include "abstractTool.h"


class QComboBox;
class toolBox;
class hilightAnnotation;

namespace Poppler { 
  class TextBox;
};

class hilightTool : public abstractTool { 
  Q_OBJECT
	protected:
	  static QIcon icon;
	  QComboBox *typSelect;
	  bool editingHilight; // if true, mouse movement edits the extent
	                       // of the current hilight

	protected:
	  void updateCurrentAnnotation( QPointF ScenePos );
          void editAnnotationExtent( abstractAnnotation *item );
	  virtual void prepareEditItem( abstractAnnotation *item );

	protected slots:
	  void updateTyp( const QString &typ );

	public:
		hilightTool( pdfScene *Scene, toolBox *ToolBar, QStackedWidget *EditArea);

		virtual abstractAnnotation *processAnnotation( PoDoFo::PdfDocument *doc, PoDoFo::PdfAnnotation *annotation, pdfCoords *transform );
		virtual void newActionEvent( const QPointF *scPos );
		virtual bool acceptEventsFor( QGraphicsItem *item );
		virtual bool handleEvent( viewEvent *ev );
		
		

		friend class hilightAnnotation;

};

class hilightAnnotation : public abstractAnnotation {
	protected:
		QList<QRectF> hBoxes;
		QRectF bBox;
		QPainterPath exactShape;
		
		/* Returns true if \param tp is one of
		 *   eHighlight, eStrikeOut, eSquiggly, eUnderline
		 * otherwise returns false. */
		static bool isMarkupAnnotation( const enum abstractAnnotation::eAnnotationTypes tp ); 
		
		
	public:
		hilightAnnotation( hilightTool *tool, enum abstractAnnotation::eAnnotationTypes tp = abstractAnnotation::eHilight, PoDoFo::PdfAnnotation *hilightAnnot = NULL, pdfCoords *transform = NULL );
		~hilightAnnotation() {};

		void updateSelection( QList<Poppler::TextBox*> newSelection );

		void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget );
		QRectF boundingRect() const {return bBox;};
		QPainterPath shape() const;

		static bool isA( PoDoFo::PdfAnnotation *annotation );
		virtual PoDoFo::PdfAnnotation *saveToPdfPage( PoDoFo::PdfDocument *document, PoDoFo::PdfPage *pg, pdfCoords *coords );
                
		/* setTyp calls prepareGeometryChange, updates the annotType to \param tp, and, if necessary,
		 * calls update to redraw the annotation. tp must be a Text Markup Annotation, i.e. one of
		 * eHighlight, eStrikeOut, eSquiggly, eUnderline, otherwise the method does nothing. */
		void setTyp(const abstractAnnotation::eAnnotationTypes tp);

		friend class hilightTool;

};



#endif /* _hilightTool_H */
