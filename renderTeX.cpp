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
#include <QtCore/QDebug>


#include "teXjob.h"
#include "renderTeX.h"

QCache<int, struct renderTeX::cachedPage> renderTeX::renderCache(20000);

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

void renderTeX::deleteItem( int item ) { 
  Q_ASSERT( 0 <= item && item < items.size() );
  delete items[item];
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

QPixmap renderTeX::render( int item, bool format_inline, qreal zoom ) { 
  Q_ASSERT( 0 <= item && item < items.size() && items[item] );
  struct cachedPage *pg = renderCache.object( item );
  if ( pg ) { 
    if ( pg->format_inline == format_inline && pg->zoom == zoom ) return pg->pix;
    if ( compileJob::pathsOK() ) { 
      pg->pix = items[item]->render( zoom, format_inline );
      pg->format_inline = format_inline;
      pg->zoom = zoom;
      return pg->pix;
    } else { 
      qWarning() << "renderTeX::render: PdfLaTex or GhostScript not found.";
      return QPixmap();
    }
  } else { 
    if ( compileJob::pathsOK() ) { 
      pg = new cachedPage;
      pg->pix = items[item]->render( zoom, format_inline );
      qDebug() << "Rendered PixMap: "<< pg->pix.width() << " x "<< pg->pix.height();
      pg->format_inline = format_inline;
      pg->zoom = zoom;
      renderCache.insert( item, pg, (int) (items[item]->size()*zoom) );
      return pg->pix; // FIXME: can crash if cache is too small and hence
                      // deletes pg imediately
    } else { 
      qWarning() << "renderTeX::render: PdfLaTex or GhostScript not found.";
      return QPixmap();
    }
  }
}

void renderTeX::preRender( int item, bool format_inline ) { 
  Q_ASSERT( 0 <= item && item < items.size() && items[item] );
  if ( compileJob::pathsOK() ) { 
    connect( items[item], SIGNAL(renderingReady(int)), this, SIGNAL(itemReady(int)) );
    items[item]->preRender( item );
  } else { 
    qWarning() << "renderTeX::render: PdfLaTex or GhostScript not found.";
  }
}



void renderTeX::renderingFinished( int i ) { 
  Q_ASSERT( 0 <= i && i < items.size() && items[i] );
  emit itemReady( i );
}
 


  

#include "renderTeX.moc"







