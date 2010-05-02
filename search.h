#ifndef _search_H
#define _search_H

/**  This file is part of comment
*
*  File: search.h
*  Created: 13. 02. 2009
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
