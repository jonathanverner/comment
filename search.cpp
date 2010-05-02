/**  This file is part of project comment
 *
 *  File: search.cpp
 *  Created: 2009-02-13
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





#include "search.h"
#include "pdfScene.h"
#include "sceneLayer.h"
#include "hiliteItem.h"

#include <QtCore/QDebug>

#include <poppler-qt4.h>

using namespace Poppler;


searcher::searcher( pdfScene *SC ):
	scene(SC)
{
  searchLayer = scene->addLayer();
  searchLayer->setZValue(30);
}

searcher::~searcher() { 
  scene->removeLayer( searchLayer );
}

/*QRectF searcher::matchPosition( int match ) {
  return searchLayer->getItem( match )->sceneBoundingRect();
}*/

int searcher::numOfMatches( int n ) {
  return matches.size();
}

void searcher::hilightMatches() { 
  pageSelections pgSel;
  QPointF curPagePos;
  hiliteItem *hi;
  foreach( pageSelections pageMatches, matches )  {
    curPagePos = scene->topLeftPage( pageMatches.pageNum );
    foreach( QList<TextBox *> match, pageMatches.selections ) { 
      hi = new hiliteItem( curPagePos, match );
      searchLayer->addItem( hi );
    }
  }
  dynamic_cast<hiliteItem*>(searchLayer->currentItem())->setActive();
}



/* FIXME: make this smarter --- once we have found
 * say 'amer' and text == 'ameri' instead of searching
 * for ameri all over again we can go through
 * the current matches and delete all those which
 * do not start with ameri. */
void searcher::searchTermChanged( QString text ) {
  searchTerm = text;
  if ( text == "" ) { 
    clearSearch();
    return;
  }
  if ( text.size() < 3 ) { 
    matches = scene->findText( text, currentPage, currentPage + 1 );
    if ( matches.size() == 0 ) matches = scene->findText( text );
  } else matches = scene->findText( text );
  searchLayer->clear();
  if ( matches.size() > 0 ) { 
    hilightMatches();
    emit currentMatchPosition( searchLayer->currentItem()->sceneBoundingRect() );
    emit matchFound( matches.size() );
  } else { 
    emit matchNotFound();
  }
}

void searcher::clearSearch() {
  searchTerm="";
  searchLayer->clear();
  matches.clear();
  emit clear();
}

void searcher::advanceMatch( int i ) { 
  if ( matches.size() > 0 ) { 
    dynamic_cast<hiliteItem*>(searchLayer->currentItem())->setInactive();
    searchLayer->advanceCurrentItem( i );
    dynamic_cast<hiliteItem*>(searchLayer->currentItem())->setActive();
    emit currentMatchPosition( searchLayer->currentItem()->sceneBoundingRect() );
  }
}

void searcher::nextMatch() {
  advanceMatch();
}

void searcher::prevMatch() {
  advanceMatch(-1);
}

#include "search.moc"
