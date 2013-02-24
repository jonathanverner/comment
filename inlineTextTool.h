

#ifndef INLINETEXTTOOL_H
#define INLINETEXTTOOL_H

/**  This file is part of comment
*
*  File: inlineTextTool.h
*  Created: ??. 04. 2010
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
#include <QtGui/QGraphicsTextItem>
#include <QtGui/QIcon>

class toolBox;
class inlineTextAnnotation;


class inlineTextTool : public abstractTool {
  Q_OBJECT
	private:
	  static QIcon icon;

	protected:
	  renderTeX *inlineRenderer;
	  QVector<inlineTextAnnotation*> int2annot;
	  
          void editAnnotationExtent( abstractAnnotation *item );
	  void editAnnotationText();
	  void finishEditing();
	  void prepareTeX( inlineTextAnnotation *item );
	  
  protected slots:
    
    void teXReady( int item );

	public:
                inlineTextTool(pdfScene* Scene, toolBox* ToolBar, QStackedWidget* EditArea);
	        ~inlineTextTool();
		
		virtual abstractAnnotation *processAnnotation( PoDoFo::PdfDocument* doc, PoDoFo::PdfAnnotation* annotation, pdfCoords* transform );
		virtual void newActionEvent( const QPointF *scPos );
		virtual bool acceptEventsFor( QGraphicsItem *item );
		/*virtual bool handleEvent( viewEvent *ev );*/
		virtual void editItem( abstractAnnotation *item );

		friend class inlineTextAnnotation;

};

class myTextItem : public QGraphicsTextItem {
protected:
  virtual void keyPressEvent( QKeyEvent *event );
  virtual void keyReleaseEvent (QKeyEvent *event );
  friend class pageView;
};
  
class inlineTextAnnotation : public abstractAnnotation {
  Q_OBJECT
  static int inlineAnnotationCount;
  private:
    myTextItem *item;
    int inlineID;
    bool teXAppearance;
  QRectF brec;
    
  protected:
    void setTeXAppearance(bool);
   
		/* The font name to be used in the DA
		 * key of the pdf annotation dictionary.
		 * This will be the font of the annotation.
		 * Should be one of the 14 supported fonts */
		QString fontName;
    
	  
	public:
		inlineTextAnnotation( inlineTextTool *tool, PoDoFo::PdfAnnotation *hilightAnnot = NULL, pdfCoords *transform = NULL );
		~inlineTextAnnotation();


		void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget );
		QRectF boundingRect() const;
		QPainterPath shape() const;
		
		virtual void setColor( const QColor &col );

		static bool isA( PoDoFo::PdfAnnotation *annotation );
		virtual PoDoFo::PdfAnnotation *saveToPdfPage( PoDoFo::PdfDocument *document, PoDoFo::PdfPage *pg, pdfCoords *coords );


		friend class inlineTextTool;

};

#endif // INLINETEXTTOOL_H
