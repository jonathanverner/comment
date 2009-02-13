#ifndef _search_H
#define _search_H

/***************************************************************
 * search.h
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

#include <QtCore/QRectF>
#include <QtCore/QString>

#include "sceneLayer.h"
#include "pdfScene.h"

class searcher : public QObject {
  Q_OBJECT
	private:
	  pdfScene *scene;
	  sceneLayer *searchLayer;
	  int numberOfMatches, currentPage;
	  QString searchTerm;

	  QList<pageSelections> matches;


	  void hilightMatches();
	  void advanceMatch( int i = 1 );


	public:
	  searcher( pdfScene *scene );
	  ~searcher();
	  int numOfMatches( int n );

	public slots:

	  void searchTermChanged( QString text );
	  void clearSearch();

	  void nextMatch();
	  void prevMatch();


	signals:
	  void matchNotFound();
	  void matchFound( int n );
	  void clear();
          void currentMatchPosition( const QRectF &sceneRect );

};


#endif /* _search_H */
