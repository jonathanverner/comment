#ifndef _abstractTool_H
#define _abstractTool_H

/***************************************************************
 * abstractTool.h
 * @Author:      Jonathan Verner (jonathan.verner@matfyz.cz)
 * @License:     GPL v2.0 or later
 * @Created:     2008-10-17.
 * @Last Change: 2008-10-17.
 * @Revision:    0.0
 * Description:
 * Usage:
 * TODO:
 *CHANGES:
 ***************************************************************/
#include <QtCore/QString>
#include <QtCore/QDate>
#include <QtCore/QTime>

#include <QtGui/QPixmap>
#include <QtGui/QGraphicsItem>

class QToolBar;
class QStackedWidget;
class abstractAnnotation;
class QWidget;
class QPoint;
class pdfScene;
class QGraphicsSceneMouseEvent;
class QGraphicsSceneHoverEvent;
class toolBox;

class abstractTool : public QObject { 
	protected:
		QStackedWidget *editArea;
		toolBox *toolBar;
		QGraphicsScene *scene;
		QWidget *editor;
		abstractAnnotation *currentEditItem;
		QString author;

	public:
		 abstractTool( pdfScene *Scene, toolBox *ToolBar, QStackedWidget *EditArea  ): editArea(EditArea), scene(Scene), toolBar(ToolBar), currentEditItem( NULL ) {} ;
		 ~abstractTool();

		 QString getAuthor() const { return author; };
		 void setAuthor( QString Author ) { author = Author; };

		 /* Returns true, if the annotation was recognized (and added to the scene)
		  * otherwise returns false */
		 virtual bool processAnnotation( PoDoFo::PdfAnnotation *annotation, int page ) = 0;

		 /* Called when the user wants to add a new annotation at scenePos 
		  * (scene coordinates) */
		 virtual void newActionEvent( QPoint *scenePos ) = 0;


		 /* Called by an item, which wants to be edited. The item passes
		  * a reference to itself */
		 virtual void editItem( abstractAnnotation *item );

};

class abstractAnnotation : public QGraphicsItem { 
	private:
		QPixmap toolTipPixMap, icon;
		QString toolTipRichText;
		bool waitingForToolTip, haveToolTip, moved, showingToolTip;
		enum toolTipType { pixmap, text };
		toolTipType tp;

		QString author;
		QDate date;
		QTime time;

		int page;

		abstractTool *myTool;


	protected:
		void setMyToolTip(const QPixmap &pixMap);
		void setMyToolTip(const QString &richText);
		void setIcon( const QPixmap &icon);

		void mouseMoveEvent( QGraphicsSceneMouseEvent *event );
		void mousePressEvent( QGraphicsSceneMouseEvent *event );
		void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );
		void hoverEnterEvent( QGraphicsSceneHoverEvent *event );
		void hoverLeaveEvent( QGraphicsSceneHoverEvent *event );



	public:
		static bool isA( PoDoFo::PdfAnnotation *annotation ) { return false;};
		friend bool abstractTool::processAnnotation( PoDoFo::PdfAnnotation *annotation, int page );
 
	public:
		abstractAnnotation( abstractTool *tool, int page = 0, PoDoFo::PdfAnnotation *annotation = NULL );


		void setAuthor( QString a ) { author = a; };
		void setDate( QDate d ) { date = d; };
		void setTime( QTime t ) { time = t; };
		QString getAuthor() { return author; };
		QDate getDate() { return date; };
		QTime getTime() { return time; };
		int getPage() { return page; };
		/* Warning, does not update the position
		 * of the annotation on the scene !!!
		 * might be fixed in the future, thought */
		void setPage( int pg ) { page = pg; };

		virtual QRectF boundingRect() const;
		virtual void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget );



		virtual void saveToPdfPage( PoDoFo::PdfDocument *document, int page ) = 0;

};





#endif /* _abstractTool_H */
