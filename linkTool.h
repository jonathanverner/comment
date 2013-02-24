#ifndef LINKTOOL_H
#define LINKTOOL_H

/**  This file is part of comment
*
*  File: linkTool.h
*  Created: 2009
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

#include <QtGui/QIcon>

#include <QtCore/QString>

class targetItem;
class toolBox;
class linkAnnotation;
class linkLayer;
class viewEvent;

namespace PoDoFo {
    class PdfDestination;
    class PdfMemDocument;
    class PdfDocument;
}



class linkTool : public abstractTool {
    Q_OBJECT
private:
  
    static QIcon icon;
    
    void emit_gotoPos( const QPointF &pos );

public:
    linkTool( pdfScene *Scene, toolBox *ToolBar, QStackedWidget *EditArea );
    virtual abstractAnnotation *processAnnotation( PoDoFo::PdfDocument *doc, PoDoFo::PdfAnnotation* annotation, pdfCoords* transform );
    virtual void newActionEvent( const QPointF *ScenePos );
    virtual bool acceptEventsFor( QGraphicsItem *item );
    virtual bool handleEvent( viewEvent *ev );
    friend class linkAnnotation;
    
  signals:
    void gotoPos( const QPointF &pos );
};

class linkAnnotation : public abstractAnnotation {
  private:
    targetItem *tgt;
    QRectF activeArea;
  
    public:
        linkAnnotation( linkTool* tool, PoDoFo::PdfAnnotation* Link, PoDoFo::PdfDocument *doc,  pdfCoords* transform = 0 );
        static bool isA( PoDoFo::PdfAnnotation *annotation );
        virtual PoDoFo::PdfAnnotation *saveToPdfPage( PoDoFo::PdfDocument *document, PoDoFo::PdfPage *pg, pdfCoords *coords );
	virtual QRectF boundingRect() const { return activeArea; };
	virtual void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget );
	virtual bool editSelf();
	
	friend class linkTool;
};



#endif // LINKTOOL_H
