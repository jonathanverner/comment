#ifndef _sceneLayer_H
#define _sceneLayer_H

/***************************************************************
 * sceneLayer.h
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
