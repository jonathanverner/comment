/***************************************************************
 * pageTextLayer.cpp
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

#include "pageTextLayer.h"
#include "pdfUtil.h"

using namespace Poppler;

class line { 
	private:
		QString text;
		QVector<TextBox*> boxes;
		QVector<int> wordPosInText;
//		int sz,sPos,ePos;
		int getIndex( qreal pos );
		int getIndex( int textPos );
		qreal minY, maxY;
		QList<TextBox*> intervalBoxes( int s, int e );
	public:
		int sz,sPos,ePos;

		line( int SPos );
		~line();

		/* takes ownership of the box !!! */
		void add( TextBox *bx );
		template <class T> QList<TextBox*> interval( T from, T to );
		template <class T> QList<TextBox*> toEnd( T from );
		template <class T> QList<TextBox*> fromStart( T to );
		QList<TextBox*> all();
		/* returns
		 *   -1 if y is below
		 *    0 if y is contained
		 *    1 if y is above */
		int relativePosition( qreal y );
		int relativePosition( int y );
};


line::line(int SPos): sz(0), sPos(SPos), ePos(SPos), minY(-1), maxY(-1), text("") { 
  boxes.clear();
//  wordBoxes.clear();
  wordPosInText.clear();
};

line::~line() { 
  foreach( TextBox *bx, boxes ) { 
    delete bx;
  }
}


void line::add( TextBox *bx ) { 
  QString txt=bx->text();
  int tsz=txt.size()+1;
  qreal lY = bx->boundingBox().y();
  qreal uY = bx->boundingBox().y()+bx->boundingBox().height();


  wordPosInText.append( text.size()+sPos );

  /* We have too old a version, cannot do character-level selections:
   *
      for(int i=0;i<tsz;i++) rectBoxes.append( bx->charBoundingBox(i) );
      sz+=tsz;
   * 
   * For now we just do 
   */

  boxes.append( bx );
  text+=txt+" ";
  sz++;

  ePos +=tsz;
  if ( minY == -1 ) {
    minY = lY;
    maxY = uY;
  } else {
    if ( lY < minY ) minY = lY;
    if ( uY > maxY ) maxY = uY;
  }
};

int line::relativePosition( qreal y ) { 
 if ( y < minY ) return -1;
 if ( y > maxY ) return 1;
 return 0;
}

int line::relativePosition( int y ) { 
 if ( y < sPos ) return -1;
 if ( y > ePos ) return 1;
 return 0;
}


/*int line::getIndex( qreal pos ) {
  Q_ASSERT( sz > 0 );
  int ret=0;
  qreal cur_pos = rectBoxes[ret].x();
  while( cur_pos < pos && ret < sz) cur_pos = rectBoxes[ret++].x();
  if ( ret == sz ) return sz-1;
  return ret;
}*/

int line::getIndex( qreal pos ) { 
  Q_ASSERT( sz > 0 );
  int min = 0, max = sz-1, pivot=min+(max-min)/2, dist;
  qreal minPivotVal,maxPivotVal;
  while( min < max ) { 
    dist=(max-min);
    minPivotVal = boxes[pivot]->boundingBox().x();
    maxPivotVal = minPivotVal+boxes[pivot]->boundingBox().width();
    if ( pos < minPivotVal ) max = pivot;
    else if ( maxPivotVal < pos ) min = pivot;
    else return pivot;
    pivot = min+(max-min)/2;
    if ( (max-min) >=dist ) return max;
//    if ( (max-min) >=dist ) return pivot;

  }
  return pivot;
};

int line::getIndex( int textPos ) { 
  Q_ASSERT( sz > 0 );
  int min = 0, max = sz-1, pivot=min+(max-min)/2, dist;
  int minPivotVal,maxPivotVal;
  while( min < max ) { 
    dist=(max-min);
    minPivotVal = wordPosInText[pivot];
    maxPivotVal = boxes[pivot]->text().size();
    if ( textPos < minPivotVal ) max = pivot;
    else if ( maxPivotVal < textPos ) min = pivot;
    else {
      return pivot;
    }
    pivot = min+(max-min)/2;
    if ( (max-min) >=dist ) {
      return pivot;
    }
  }
  return pivot;
};



QList<TextBox*> line::all() {
  return QList<TextBox*>::fromVector( boxes );
}

template <class T> QList<TextBox*> line::toEnd( T from ) { 
  return intervalBoxes( getIndex(from), sz-1 );
};

template <class T> QList<TextBox*> line::fromStart( T to ) { 
  return intervalBoxes( 0, getIndex(to) );
};

template <class T> QList<TextBox*> line::interval( T from, T to ) { 
  return intervalBoxes( getIndex(from), getIndex(to) );
};


QList<TextBox*> line::intervalBoxes( int s, int e ) { 
 QList<TextBox*> ret;
 QString txt="";
 for(int i=s;i<=e;i++) {
   ret+=boxes[i];
//   txt+=boxes[i]->text()+" ";
 }
// qDebug() << "Selected text ("<<s<<"-"<<e<<"):"<<txt;
 return ret;
};


pageTextLayer::pageTextLayer( Page *pg ) {
  int posInText = 0;
  line *ln = new line( posInText );
  qreal lastx = 0;
  lines.clear();
  foreach( TextBox *box, pg->textList() ) { 
    if (box->boundingBox().x() < lastx ) { //newline
//     qDebug() << "Adding line: ["<<ln->sPos<<"-"<<ln->ePos<<"]";
//      pdfUtil::debugPrintTextBoxen( ln->all() );
      lines.append(ln);
      ln = new line( posInText );
    };
    ln->add( box );
    lastx = box->boundingBox().x();
    posInText += box->text().size() + 1;
    pageText += " " + box->text();
  }
}

pageTextLayer::~pageTextLayer() { 
  foreach( line *ln, lines ) { 
    delete ln;
  }
}


template <class T> int pageTextLayer::findLine( T y, int minLineHint ) { 
  int min = minLineHint, max = lines.size()-1, pivot=min+(max-min)/2;
  Q_ASSERT(max >= min); 
  if ( lines[min]->relativePosition( y ) == -1 ) return 0;
  else if ( lines[max]->relativePosition( y ) == 1 ) return max;
  while( min < max ) { 
    switch(lines[pivot]->relativePosition( y )) { 
	    case -1:
		    max = pivot;
		    break;
	    case 0:
		    return pivot;
	    case 1:
		    if ( min == pivot ) {
		      return min;
		    }
		    min = pivot;
    };
    pivot = min+(max-min)/2;
  }
  return pivot;
};



QList<TextBox*> pageTextLayer::select( QPointF from, QPointF to ) { 
 QList<TextBox*> ret;
 if ( lines.size() < 1 ) return ret;
 int startLine = findLine( from.y() ), endLine = findLine( to.y() );
 qreal slineX = from.x(), elineX = to.x();
 if ( startLine > endLine ) {
   startLine +=endLine; slineX += elineX;
   endLine = startLine-endLine; elineX = slineX-elineX;
   startLine -=endLine; slineX -=elineX;
 }
 qDebug() << "testLayer::select " << startLine << " - " << endLine;
  // The code needs to be checked for correctnes when we are in fact selecting backwards
 if ( startLine == endLine ) {
   if ( slineX < elineX ) return lines[startLine]->interval( slineX, elineX );
   else return lines[startLine]->interval( elineX, slineX );
 }
 ret = lines[startLine]->toEnd( slineX );
 for(int i=startLine+1;i<endLine;i++ ) ret += lines[i]->all();
 ret += lines[endLine]->fromStart( elineX );
 return ret;
};


QList<TextBox*> pageTextLayer::interval( int sPos, int ePos ) { 
  QList<TextBox*> ret;
  int lnS = findLine( sPos ), lnE = findLine( ePos, lnS );
  if ( lnS == lnE ) { 
    ret=lines[lnS]->interval( sPos, ePos );
  } else { 
    ret=lines[lnS]->toEnd( sPos );
    for(int i=lnS+1;i<lnE; ++i)
      ret+=lines[i]->all();
    ret+=lines[lnE]->fromStart( ePos );
  }
  return ret; 
}

QList< QList<TextBox*> > pageTextLayer::findText( QString text ) {
  QList< QList<TextBox*> > ret;
  ret.clear();
  int pos=0, foundAt=pageText.indexOf( text, pos );

  while ( foundAt >= 0 ) {
 //   qDebug() << " interval( " << foundAt << ", " << foundAt + text.size() -1 << " );";
//    pdfUtil::debugPrintTextBoxen( interval( foundAt, foundAt + text.size()-1 ) );
    ret+=interval( foundAt, foundAt + text.size()-1 );
    pos=foundAt+text.size();
    foundAt=pageText.indexOf( text, pos );
  }

  return ret;
}



