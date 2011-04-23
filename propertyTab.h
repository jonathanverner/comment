#ifndef _propertyTab_H
#define _propertyTab_H

/**  This file is part of comment
*
*  File: propertyTab.h
*  Created: 7. 1. 2009
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

#include <QtGui/QWidget>
#include <QtGui/QColor>

class QLineEdit;
class QPushButton;
class QVBoxLayout;
class QPixmap;
class QIcon;

class propertyTab : public QWidget { 
  Q_OBJECT

	  QLineEdit *authorEdit;
	  QPushButton *colorPush;
	  QVBoxLayout *layout;
	  QPixmap *pix;
	  QIcon *icon;

          QColor color;

	protected:
	  void addWidgetWithLabel( QString Label, QWidget *w );
	  
        protected slots:
	  void inputColor();

	public:
	  propertyTab( QWidget *parent = 0 );

	  void setAuthor( QString Author );
	  void setColor( QColor color );

	  QString getAuthor();
	  QColor getColor();

	signals:
	  void authorChanged();
	  void colorChanged();
  
};


#endif /* _propertyTab_H */
