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


#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QAbstractItemModel>
#include <QtCore/QVariant>

class targetItem;
class linkLayer;

namespace PoDoFo {
  class PdfMemDocument;
  class PdfDocument;
  class PdfOutlineItem;
class PdfDocument;
};

class tocItem {

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
    int getPage() const;
    targetItem *getTarget() { return tgt; };
    void setTarget( targetItem *target ) { tgt = target;};
    QVariant data( int column ) const;
    
    int columnCount() const { return 2; };
    
    tocItem *parentItem() { return parent; }
    
    tocItem *child(int row);
    int childCount() const;
    int row() const;
    

    QList<tocItem *>::iterator begin() { return children.begin(); };
    QList<tocItem *>::iterator end() { return children.end(); };
};

class toc : public QAbstractItemModel {

  Q_OBJECT
  
  private:
    
    tocItem *root;
    linkLayer *links;
    
    tocItem* loadOutlineItem( PoDoFo::PdfOutlineItem* item, tocItem* parent, const QString& path, PoDoFo::PdfDocument *doc );
  
  public:
    
    toc( linkLayer* Links, PoDoFo::PdfMemDocument *doc = NULL );
    ~toc();
    
    QVariant data( const QModelIndex &index, int role ) const;
    Qt::ItemFlags flags( const QModelIndex &index ) const;
    QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    QModelIndex index( int row, int column, const QModelIndex &parent = QModelIndex() ) const;
    QModelIndex parent( const QModelIndex &index ) const;
    int rowCount( const QModelIndex &parent = QModelIndex() ) const;
    int columnCount( const QModelIndex &parent = QModelIndex() ) const;
    
    void load( PoDoFo::PdfMemDocument* doc );
    void save( PoDoFo::PdfMemDocument* doc );
    
    tocItem *getRoot() { return root; };
    tocItem *getItem( const QModelIndex &index );
    
  signals:
    
    void tocChanged();
    
};


#endif // _toc_H
