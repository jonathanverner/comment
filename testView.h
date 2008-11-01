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
#include <QtCore/QPoint>

class QGraphicsScene;
class QMouseEvent;

class testView : public QGraphicsView { 
  Q_OBJECT
	private:
		qreal zoom;
		int currentPage;
		bool moving, moved;
	public:
		testView( QGraphicsScene *scene, QWidget *parent = 0 ) : QGraphicsView(scene, parent), zoom(1), currentPage(1), moving(false), moved(false) { setDragMode( QGraphicsView::ScrollHandDrag ); };
		int getCurrentPage();
	signals:
		void mouseNearBorder( const QPoint &pos );
		void onPage( int num );
		void newAnnotationAction( const QPointF &scenePos );
	public slots:
	  void zoomIN();
	  void zoomOUT();
          void up();
	  void down();
	  void left();
	  void right();
	  void nextPage();
	  void prevPage();
	  void gotoPage( int num );

	protected:
	  virtual void mouseMoveEvent( QMouseEvent *e );
	  virtual void mouseReleaseEvent( QMouseEvent *e );
	  virtual void mousePressEvent( QMouseEvent *e );
};




#endif /* _testView_H */
