#ifndef _toc_H
#define _toc_H

/**  This file is part of comment
*
*  File: toc.h
*  Created: 2. 5. 2010
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


/**  This file is part of comment
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

#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QObject>

class targetItem;
class linkLayer;

namespace PoDoFo {
  class PdfDocument;
  class PdfOutlineItem;
};

class tocItem : public QObject {
  Q_OBJECT
  
  private:
    
    QString title;
    tocItem *parent;
    QList<tocItem *> children;
    targetItem *tgt;
    
  public:
    
    tocItem( const QString Title, tocItem *Parent = NULL, targetItem *Tgt = NULL ): title(Title), parent(Parent), tgt(Tgt) {};
    ~tocItem();
    
    void appendChild( tocItem *child );
    void insertChild( tocItem *after, tocItem *child );
    void removeChild( tocItem *child );
    
    QString getTitle() const { return title; };
    void setTitle( const QString& ttl ) { title = ttl; };
    
    QList<tocItem *>::iterator begin() { return children.begin(); };
    QList<tocItem *>::iterator end() { return children.end(); };
};

class toc : public QObject {

  Q_OBJECT
  
  private:
    
    tocItem *root;
    linkLayer *links;
    
    tocItem* loadOutlineItem( PoDoFo::PdfOutlineItem* item, tocItem* parent, const QString& path );
  
  public:
    
    toc( PoDoFo::PdfDocument* doc, linkLayer* links );
    void save( PoDoFo::PdfDocument* doc );
    
    tocItem *getRoot() { return root; };
    
  signals:
    
    void tocChanged();
    
};


#endif // _toc_H