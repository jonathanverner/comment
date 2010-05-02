
/**  This file is part of project comment
 *
 *  File: toc.cpp
 *  Created: 2. 5. 2010
 *  Author: Jonathan Verner <jonathan.verner@matfyz.cz>
 *  Licence: GPL v2 or later
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

void tocItem::removeChild ( tocItem* child ) {
  children.removeOne( child );
  delete child;
}

tocItem::~tocItem() {
  foreach( tocItem *child, children ) {
    delete child;
  };
};




toc::toc ( PoDoFo::PdfDocument* doc, linkLayer* Links ): links(Links) {
  PdfOutlineItem *toc_root = doc->GetOutlines();
  if ( ! toc_root ) return;
  PdfOutlineItem *child = toc_root->First();
  root = new tocItem( tr("Table of Contents") );
  while( child ) {
    tocItem *item = loadOutlineItem( child, root, "toc" );
    root->appendChild( root );
    child = child->Next();
  }
};

tocItem* toc::loadOutLineItem( PdfOutlineItem* item, tocItem* parent, const QString& path ) { 
    PdfDestination *dest = item->GetDestination();
    QString title = item->GetTitle();
    targetItem *tgt = links->addTarget( path+"/"+title, dest->GetPage()->GetPageNumber()-1, QRectF(0,0,0,0) );
    tocItem *me = new tocItem( title, parent, tgt );
    PdfOutlineItem *child = item->First();
    while( child ) {
      tocItem *item = loadOutLineItem( child, me, path+"/"+title );
      me->appendChild( item );
      child = child->Next();
    }
    return me;
};

toc::save ( PoDoFo::PdfDocument* doc )
{

}





#include "toc.moc"