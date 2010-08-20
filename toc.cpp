
/**  This file is part of project comment
 *
 *  File: toc.cpp
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

#include "toc.h"
#include "linkLayer.h"

#include "pdfUtil.h"

#include <podofo/podofo.h>



using namespace PoDoFo;

void tocItem::appendChild ( tocItem* child ) {
  children.append(child);
}

void tocItem::insertChild( tocItem* after, tocItem* child ) {
  if ( ! after ) {
    children.prepend( child );
    return;
  }
    
  for( QList<tocItem *>::iterator it = children.begin(); it != children.end(); ++it ) {
    if (*it == after ) {
      children.insert( it, child );
      break;
    }
  }
}

void tocItem::removeChild ( tocItem* child ) {
  children.removeOne( child );
  delete child;
}

tocItem::~tocItem() {
  qDeleteAll(children);
};




toc::toc( linkLayer* Links, PoDoFo::PdfMemDocument* doc ): links(Links), root(NULL) {
  load( doc );
};

toc::~toc() {
  delete root;
}

void toc::load( PoDoFo::PdfMemDocument* doc ) {
  if ( ! doc ) return;
  PdfOutlineItem *toc_root = doc->GetOutlines();
  if ( ! toc_root ) return;
  PdfOutlineItem *child = toc_root->First();
  delete root;
  root = new tocItem( tr("Table of Contents") );
  while( child ) {
    tocItem *item = loadOutlineItem( child, root, "toc" );
    root->appendChild( item );
    child = child->Next();
  }
}

tocItem* toc::loadOutlineItem( PdfOutlineItem* item, tocItem* parent, const QString& path ) { 
    PdfDestination *dest = pdfUtil::getDestination(item);
    QString title = pdfUtil::pdfStringToQ( item->GetTitle() );
    qDebug() << "Loading TOC item: " << path << "." << title;
    targetItem *tgt = NULL;
    QString nm = path+"."+title;
    if ( dest ) tgt = links->addTarget( nm, dest );
    else tgt = NULL;
    tocItem *me = new tocItem( title, parent, tgt );
    PdfOutlineItem *child = item->First();
    while( child ) {
      tocItem *item = loadOutlineItem( child, me, path+"."+title );
      me->appendChild( item );
      child = child->Next();
    }
    return me;
};

void toc::save ( PoDoFo::PdfMemDocument* doc )
{

}

QVariant tocItem::data(int column) const {
    if ( column == 0 ) return getTitle();
    else if ( column == 1 ) return getPage();
    else return QVariant();

}
int tocItem::row() const {
    if ( parent ) 
      return parent->children.indexOf(const_cast<tocItem*>(this)); 
    return 0;
}

tocItem* tocItem::child(int row) {
    return children.value(row);
}

int tocItem::childCount() const {
    return children.count();
}

int tocItem::getPage() const {
  if ( tgt ) return tgt->getPage();
  else return 0;
}



QModelIndex toc::index(int row, int column, const QModelIndex& parent) const {
  if ( ! hasIndex( row, column, parent ) ) return QModelIndex();
  
  tocItem *pItem;
  
  if ( ! parent.isValid() ) pItem = root;
  else pItem = static_cast<tocItem *>(parent.internalPointer());
  
  tocItem *chItem = pItem->child(row);
  if ( chItem ) return createIndex( row, column, chItem );
  else return QModelIndex();
}

QModelIndex toc::parent(const QModelIndex& index) const {
  if ( ! index.isValid() ) return QModelIndex();
  
  tocItem *chItem = static_cast<tocItem *>(index.internalPointer());
  tocItem *pItem = chItem->parentItem();
  
  if ( pItem == root ) return QModelIndex();
  
  return createIndex( pItem->row(), 0, pItem );
}

int toc::rowCount(const QModelIndex& parent) const {
  tocItem * pItem;
  if ( parent.column() > 0 ) return 0;
  if ( ! parent.isValid() ) pItem = root;
  else pItem = static_cast<tocItem *>(parent.internalPointer());
  return pItem->childCount();
}


int toc::columnCount(const QModelIndex& parent) const {
  if ( parent.isValid() ) return static_cast<tocItem *>(parent.internalPointer())->columnCount();
  else return root->columnCount();
}


QVariant toc::data(const QModelIndex& index, int role) const {

  if ( !index.isValid() ) return QVariant();
  
  tocItem *item = static_cast<tocItem *>(index.internalPointer());

  if ( role == Qt::ToolTipRole ) {
    QString ret = "Page "+ QString::number(item->getPage()+1);
    return ret;
  }
  
  if ( role == Qt::DisplayRole ) {
    return item->data(index.column());
  }

  return QVariant();

}

tocItem* toc::getItem(const QModelIndex& index ) {
  if ( !index.isValid() ) return NULL;
  return static_cast<tocItem *>(index.internalPointer());
}


Qt::ItemFlags toc::flags(const QModelIndex& index) const {
  if ( ! index.isValid() ) return 0;  
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant toc::headerData(int section, Qt::Orientation orientation, int role) const {
  if ( orientation != Qt::Horizontal || role != Qt::DisplayRole ) return QVariant();
  switch (section) { 
    case 0:
      return "Table of Contents";
      break;
    case 1:
      return "Page";
      break;
  };
}



#include "toc.moc"