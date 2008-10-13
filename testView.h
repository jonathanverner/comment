#ifndef _testView_H
#define _testView_H

/***************************************************************
 * testView.h
 * @Author:      Jonathan Verner (jonathan.verner@matfyz.cz)
 * @License:     GPL v2.0 or later
 * @Created:     2008-10-12.
 * @Last Change: 2008-10-12.
 * @Revision:    0.0
 * Description:
 * Usage:
 * TODO:
 *CHANGES:
 ***************************************************************/

#include <QtGui/QGraphicsView>

class QGraphicsScene;
class QWidget;

class testView : public QGraphicsView { 
  Q_OBJECT
	private:
		qreal zoom;
	public:
		testView( QGraphicsScene *scene, QWidget *parent = 0 ) : QGraphicsView(scene, parent), zoom(1) {};
	public slots:
	  void zoomIN();
	  void zoomOUT();
          void up();
	  void down();
	  void left();
	  void right();
};




#endif /* _testView_H */
