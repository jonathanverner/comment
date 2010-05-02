/***************************************************************
 * renderTeX.cpp
 * @Author:      Jonathan Verner (jonathan.verner@matfyz.cz)
 * @License:     GPL v2.0 or later
 * @Created:     2008-11-17.
 * @Last Change: 2008-11-17.
 * @Revision:    0.0
 * Description:
 * Usage:
 * TODO:
 *CHANGES:
 ***************************************************************/


#include "teXjob.h"
#include "renderTeX.h"

#include <QtCore/QDebug>

QCache<int, struct renderTeX::cachedPage> renderTeX::renderCache(200000);

renderTeX::renderTeX( QString preamb ):
	preambule( preamb )
{
}

void renderTeX::setPaths( QString latex, QString gs ) { 
  compileJob::setPaths( latex, gs );
}

void renderTeX::setPreambule( QString preamb ) { 
  preambule = preamb;
}

/* This method should always succeed */
int renderTeX::addItem( QString source, QString preamb ) { 
  if ( preamb == "" ) preamb = preambule;
  renderItem *it = new renderItem( source, preamb );
  int id;
  if ( available_ids.size() > 0 ) { 
    id = available_ids.pop();
    items[id] = it;
  } else {
    items.append( it );
    id = items.size()-1;
  }
  return id;
}

void renderTeX::setItem( int itemID, QString source, QString preamb ) {
  if ( preamb == "" ) preamb = preambule;
  renderItem *it = new renderItem( source, preamb );
  if ( items.size() <= itemID ) items.resize( itemID + 10 );
  else {
    deleteItem( itemID );
    available_ids.pop();
  }
  items[itemID] = it;
}
  

void renderTeX::deleteItem( int item ) { 
  Q_ASSERT( 0 <= item && item < items.size() );
  delete items[item];
  renderCache.remove( item );
  items[item]=NULL;
  available_ids.push(item);
}

void renderTeX::updateItem( int item, QString source, QString preamb ) { 
  Q_ASSERT( 0 <= item && item < items.size() && items[item] );
  if ( preamb == "" ) preamb = preambule;
  connect( items[item], SIGNAL(renderingReady(int)), this, SIGNAL(itemReady(int)) );
  items[item]->updateItem( source, preamb, item );
  renderCache.remove( item );
}

QPixmap renderTeX::render( int item, bool format_inline, qreal zoom, int sizeHint ) { 
  Q_ASSERT( 0 <= item && item < items.size() && items[item] );
  struct cachedPage *pg = renderCache.object( item );
  if ( pg ) { 
    if ( pg->format_inline == format_inline && pg->zoom == zoom ) return pg->pix;
    if ( compileJob::pathsOK() ) { 
      pg->pix = items[item]->render( zoom, format_inline, sizeHint );
      pg->format_inline = format_inline;
      pg->zoom = zoom;
      return pg->pix;
    } else { 
      qWarning() << "renderTeX::render: PdfLaTex or GhostScript not found.";
      return QPixmap();
    }
  } else { 
    if ( compileJob::pathsOK() ) { 
      QPixmap ret = items[item]->render( zoom, format_inline, sizeHint );
      int cost = (int) (items[item]->size()*zoom);
      if ( cost >= renderCache.maxCost() ) { 
	qWarning() << "Warning, render cache too small, result will not be cached !!!";
	return ret;
      } else {
	pg = new cachedPage;
        pg->pix = ret;
	pg->format_inline = format_inline;
	pg->zoom = zoom;
	renderCache.insert( item, pg, (int) (items[item]->size()*zoom) );
	return ret;
      }
    } else { 
      qWarning() << "renderTeX::render: PdfLaTex or GhostScript not found.";
      return QPixmap();
    }
  }
}

void renderTeX::preRender( int item, bool format_inline, int sizeHint ) { 
  Q_ASSERT( 0 <= item && item < items.size() && items[item] );
  if ( compileJob::pathsOK() ) { 
    connect( items[item], SIGNAL(renderingReady(int)), this, SIGNAL(itemReady(int)) );
    items[item]->preRender( item, format_inline, sizeHint );
  } else { 
    qWarning() << "renderTeX::render: PdfLaTex or GhostScript not found.";
  }
}

QString renderTeX::getPDF(int item) {
  Q_ASSERT( 0 <= item && item < items.size() && items[item] );
  return items[item]->getPDFFileName();
}

QRectF renderTeX::getBBox(int item) {
  Q_ASSERT( 0 <= item && item < items.size() && items[item] );
  return items[item]->getBBox();
}



void renderTeX::renderingFinished( int i ) { 
  Q_ASSERT( 0 <= i && i < items.size() && items[i] );
  emit itemReady( i );
}
 


  

#include "renderTeX.moc"







