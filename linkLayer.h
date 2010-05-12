#ifndef _linkLayer_H
#define _linkLayer_H

/**  This file is part of comment
*
*  File: linkLayer.h
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

#include "sceneLayer.h"

#include <QtGui/QGraphicsItem>
#include <QtCore/QRectF>
#include <QtCore/QHash>
#include <QtCore/QString>

class pdfScene;

class QSignalMapper;

namespace PoDoFo {
  class PdfDestination;
};

class targetItem : public QGraphicsObject {
  Q_OBJECT
  private:
    QRectF brect;
    QString name;
    int pgNum;
    
  public:
    targetItem( int Page, const QSizeF &Area, const QString &Name = "" ): brect(QPointF(0,0),Area), name(Name), pgNum(Page) {};
    QRectF boundingRect() const { return brect; };
    
    int getPage() const { return pgNum; };
    QString getName() const { return name; };
    
    virtual void paint( QPainter*, const QStyleOptionGraphicsItem*, QWidget* ) {};
    
  public slots:
    
    void activate();
    
  signals:
    
    void activated();
};

class linkLayer : public sceneLayer {
  
  Q_OBJECT

  private:
    int generation;
    QSignalMapper *mapper;
    QHash<QString, targetItem *> targets;
    
    QString generateName();
    
  private slots:
    
    void emitGOTO( const QString &name );

  public:
    
    linkLayer( pdfScene* sc );

    targetItem *addTarget( QString& name, const int page, const QRectF& target );
    targetItem *addTarget( QString& name, PoDoFo::PdfDestination* dest );
    
    void removeTarget( const QString &name );
    
  signals:
    
    void gotoTarget( QGraphicsItem *target );

};


#endif //_linkLayer_H