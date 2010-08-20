#ifndef _pageView_H
#define _pageView_H

/***************************************************************
 * pageView.h
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
#include <QtGui/QMouseEvent>
#include <QtCore/QPoint>
//#include "toolTips.h"

class QGraphicsScene;
class abstractTool;
class QGraphicsItem;
class QKeyEvent;
class abstractAnnotation;
class hiliteItem;

class viewEvent { 
	public:
		enum eventType { VE_MOUSE_MOVE, VE_MOUSE_PRESS, VE_MOUSE_RELEASE, VE_KEY };
	private:
		eventType myType;
		QPoint evPos;
		QPoint lastEvPos;
		QPointF mousePressPosition;
		qreal distance;
		QPointF lastSP;
		QPointF SP;
		QGraphicsScene *SC;
		QGraphicsItem *IT;
		abstractAnnotation *topMostAll;
		Qt::MouseButton bt_caused;
		Qt::MouseButtons bt_state;
		QWidget *viewPort;

		QPoint eventDelta() { return lastEvPos-evPos; };
		
		QMouseEvent *original_event;

	public:

		Qt::MouseButton btnCaused() { return bt_caused; };
		Qt::MouseButtons btnState() { return bt_state; };
		QPointF  mousePressPos() { return mousePressPosition; };
		QPointF lastScenePos() { return lastSP;};
		QPointF scenePos() { return SP;};
		QPointF sceneDelta() { return SP-lastSP; };
		QGraphicsScene *scene() { return SC;};
		QGraphicsItem *item() { return IT;};
		abstractAnnotation *topItem() { return topMostAll;};
		eventType type() { return myType;};
		bool isClick();
		
		QMouseEvent *getOriginalEvent() { return original_event; };

		friend class pageView;
};


class pageView : public QGraphicsView { 
  Q_OBJECT
	private:
		qreal zoom;
		int currentPage;
		QPoint lastMouseEvPos, mousePressStartPos;
		abstractAnnotation *toolTipItem;
		QGraphicsItem *movingItem;
		QPointF moveDelta;
		abstractTool *currentTool;
		hiliteItem *hi;
		QString selectedText;

	protected:
	  viewEvent eventToVE( QMouseEvent *e, viewEvent::eventType tp );
	  virtual void mouseMoveEvent( QMouseEvent *e );
	  virtual void mouseReleaseEvent( QMouseEvent *e );
	  virtual void mousePressEvent( QMouseEvent *e );
	  //virtual void keyPressEvent( QKeyEvent *e );
	  int getLastPage();


	public:
		pageView( QGraphicsScene *scene, QWidget *parent = 0 );
//	        ~pageView();

		QAction *newAction( QString shortCut, QObject *target, const char * ); 

	signals:
		void mouseNearBorder( const QPoint &pos );
		void onPage( int num );
		void newAnnotationAction( const QPointF &scenePos );


	public slots:
	  void disableActions( bool );
	  void zoomIN();
	  void zoomOUT();
          void up();
	  void down();
	  void left();
	  void right();
	  void nextPage();
	  void prevPage();
	  void lastPage();
	  void firstPage();
	  void gotoPage( int num );
	  void gotoPoint( const QPointF &point );
	  void setCurrentTool( abstractTool *curTool );

};




#endif /* _pageView_H */
