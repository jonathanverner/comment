#ifndef _sceneLayer_H
#define _sceneLayer_H

/**  This file is part of comment
*
*  File: sceneLayer.h
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



#include <QtCore/QObject>
#include <QtCore/QList>

class QGraphicsScene;
class QGraphicsItem;

class sceneLayer : public QObject { 
  Q_OBJECT
	private:
	  QGraphicsScene *scene;
	  QList< QGraphicsItem *> items;
	  QList< bool > reference;
	  int cItem;
	  int zVal;

	public:

	  sceneLayer( QGraphicsScene *scene);
	  void setZValue(int zVal);
	  void addItem( QGraphicsItem *item, bool addToScene = true );

	  QGraphicsItem *currentItem();

	public slots:

          void advanceCurrentItem( int i = 1 );

	  void hide();
	  void show();
	  void clear();

};

#endif /* _sceneLayer_H */
