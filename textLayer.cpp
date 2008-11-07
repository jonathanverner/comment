/***************************************************************
 * textLayer.cpp
 * @Author:      Jonathan Verner (jonathan.verner@matfyz.cz)
 * @License:     GPL v2.0 or later
 * @Created:     2008-11-07.
 * @Last Change: 2008-11-07.
 * @Revision:    0.0
 * Description:
 * Usage:
 * TODO:
 *CHANGES:
 ***************************************************************/
#include <QtCore/QDebug>

#include <poppler-qt4.h>

#include "textLayer.h"

using namespace Poppler;

class line { 
	private:
		QVector<TextBox*> boxes;
		QVector<QString> textBoxes;
		QVector<QRectF> rectBoxes;
		int sz;
		int getIndex( qreal pos );
		qreal minY, maxY;
		QList<QRectF> intervalBoxes( int s, int e );
	public:
		line();
		void add( TextBox *bx );
		QList<QRectF> interval( qreal from, qreal to );
		QList<QRectF> toEnd( qreal from );
		QList<QRectF> fromStart( qreal to );
		QList<QRectF> all();
		/* returns
		 *   -1 if y is below
		 *    0 if y is contained
		 *    1 if y is above */
		int relativePosition( qreal y );
};


line::line(): sz(0), minY(-1), maxY(-1) { 
  boxes.clear();
};

void line::add( TextBox *bx ) { 
  boxes.append( bx );
  textBoxes.append( bx->text() );
  rectBoxes.append( bx->boundingBox() );
  qreal lY = bx->boundingBox().y();
  qreal uY = bx->boundingBox().y()+bx->boundingBox().height();
  if ( minY == -1 ) {
    minY = lY;
    maxY = uY;
  } else {
    if ( lY < minY ) minY = lY;
    if ( uY > maxY ) maxY = uY;
  }
  sz++;
};

int line::relativePosition( qreal y ) { 
 if ( y < minY ) return -1;
 if ( y > maxY ) return 1;
 return 0;
}

int line::getIndex( qreal pos ) {
  Q_ASSERT( sz > 0 );
  int ret=0;
  qreal cur_pos = rectBoxes[ret].x();
  while( cur_pos < pos && ret < sz) cur_pos = rectBoxes[ret++].x();
  if ( ret == sz ) return sz-1;
  return ret;
}

QList<QRectF> line::all() {
  return QList<QRectF>::fromVector( rectBoxes );
}

QList<QRectF> line::toEnd( qreal from ) { 
  return intervalBoxes( getIndex(from), sz );
};

QList<QRectF> line::fromStart( qreal to ) { 
  return interval( 0, getIndex(to) );
};
QList<QRectF> line::interval( qreal from, qreal to ) { 
  return intervalBoxes( getIndex(from), getIndex(to) );
};


QList<QRectF> line::intervalBoxes( int s, int e ) { 
 QList<QRectF> ret;
 for(int i=s;i<e;i++) {
   ret+=rectBoxes[i];
 }
 return ret;
};


textLayer::textLayer( Page *pg ) {
  lines.clear();
  line *ln = new line();
  qreal lastx = 0;
  foreach( TextBox *box, pg->textList() ) { 
    if (box->boundingBox().x() < lastx ) { //newline
      lines.append(ln);
      ln = new line();
    };
    ln->add( box );
    lastx = box->boundingBox().x();
  }
}

int textLayer::findLine( qreal y ) { 
  int min = 0, max = lines.size(), pivot=min+(max-min)/2;
  int relPos;
  Q_ASSERT(max > min); 
  if ( lines[min]->relativePosition( y ) == -1 ) return 0;
  else if ( lines[max-1]->relativePosition( y ) == 1 ) return max-1;
  while( min < max ) { 
    switch(lines[pivot]->relativePosition( y )) { 
	    case -1:
		    max = pivot;
		    break;
	    case 0:
		    return pivot;
	    case 1:
		    if ( min == pivot ) return min;
		    min = pivot;
    };
    pivot = min+(max-min)/2;
  }
};

QList<QRectF> textLayer::select( QPointF from, QPointF to ) { 
 int startLine = findLine( from.y() ), endLine = findLine( to.y() );
 if ( startLine > endLine ) {
   startLine +=endLine;
   endLine = startLine-endLine;
   startLine -=endLine;
 }
 qDebug() << "testLayer::select " << startLine << " - " << endLine;
 QList<QRectF> ret;
 if ( startLine == endLine ) return lines[startLine]->interval( from.x(), to.x() );
 ret = lines[startLine]->toEnd( from.x() );
 for(int i=startLine+1;i<endLine;i++ ) ret += lines[i]->all();
 ret += lines[endLine]->fromStart( to.x() );
 return ret;
};


