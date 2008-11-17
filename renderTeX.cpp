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
#include <QtCore/QDebuq>

#include "teXjob.h"
#include "renderTeX.h"


QCache<int, struct renderTeX::cachedPage> pdfPageItem::renderCache(20000);

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
  items[item]->updateItem( source, preamb );
  renderCache.remove( item );
}

QPixmap renderTeX::renderItem( int item, bool format_inline, qreal zoom ) { 
  Q_ASSERT( 0 <= item && item < items.size() && items[item] );
  struct cachedPage *pg = renderCache.object( item );
  if ( pg ) { 
    if ( pg->format_inline = format_inline && pg->zoom = zoom ) return pg->pix;
    if ( compileJob::pathsOK() ) { 
      pg->pix = items[item]->renderItem( zoom, format_inline, pdfLaTeXPath, ghostScriptPath );
      pg->format_inline = format_inline;
      pg->zoom = zoom;
      return pg->pix;
    } else { 
      qWarning() << "renderTeX::renderItem: PdfLaTex or GhostScript not found.";
      return QPixmap();
    }
  } else { 
    if ( compileJob::pathsOK() ) { 
      pg = new cachedPage;
      pg->pix = items[item]->renderItem( zoom, format_inline );
      pg->format_inline = format_inline;
      pg->zoom = zoom;
      renderCache.insert( item, pg, items[item]->size()*zoom );
      return pg->pix;
    } else { 
      qWarning() << "renderTeX::renderItem: PdfLaTex or GhostScript not found.";
      return QPixmap();
    }
  }
}


  

#include "renderTeX.moc"







