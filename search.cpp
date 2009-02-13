/***************************************************************
 * search.cpp
 * @Author:      Jonathan Verner (jonathan.verner@matfyz.cz)
 * @License:     GPL v2.0 or later
 * @Created:     2009-02-13.
 * @Last Change: 2009-02-13.
 * @Revision:    0.0
 * Description:
 * Usage:
 * TODO:
 *CHANGES:
 ***************************************************************/


#include <QtCore/QDebug>

#include <poppler-qt4.h>

#include "search.h"
#include "pdfScene.h"
#include "sceneLayer.h"
#include "hiliteItem.h"

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
