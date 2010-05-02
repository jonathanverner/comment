#ifndef _searchBar_H
#define _searchBar_H

/**  This file is part of comment
*
*  File: searchBar.h
*  Created: 31. 1. 2009
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

#include <QtCore/QString>
#include <QtGui/QWidget>

class QLineEdit;
class QPushButton;

class searchBar : public QWidget { 
  Q_OBJECT
	private:
	  QLineEdit *edit;
	  QPushButton *next,*prev;

	public:
		searchBar( QWidget *parent );

	public slots:


		void setText( QString text );
		void setFound();
		void setMissed();
		void setNone();
		void setNumOfMatches(int n);
		void focus();

	signals:

		void textChanged( QString text );
		void nextMatch();
		void prevMatch();

};


#endif /* _searchBar_H */
