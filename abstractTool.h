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
#include <QtGui/QGraphicsObject>
#include <QtGui/QColor>

class QToolBar;
class QStackedWidget;
class abstractAnnotation;
class renderTeX;
class QWidget;
class QPoint;
class pdfScene;
class QGraphicsSceneMouseEvent;
class QGraphicsSceneHoverEvent;
class toolBox;
class viewEvent;
class QLabel;
class QMenu;
class QTabWidget;
class QTextEdit;
class propertyTab;

namespace PoDoFo { 
  class PdfAnnotation;
  class PdfDocument;
  class PdfPage;
  class PdfAnnotation;
}

class pdfCoords;

class abstractTool : public QObject { 
  Q_OBJECT
	private:
		QString toolName;
		QString author;
		QColor color;

		renderTeX *renderer;
		QVector<abstractAnnotation *> int2annot;
		QString selectedText;

	protected:
		QStackedWidget *editArea;
		toolBox *toolBar;
		pdfScene *scene;
		QTabWidget *editor;
		QTextEdit *contentEdit;
		propertyTab *propertyEdit;
		QMenu *cntxMenu;
		abstractAnnotation *currentEditItem;


		void setToolName( QString ToolName ) { toolName = ToolName; };

		void setTeXToolTip( abstractAnnotation *annotation );
		int getApproxWidth( QString text );

	protected slots:
		void deleteCurrentAnnotation();
		void editCurrentAnnotationProperties();
		void updateContent();
		void updateAuthor();
		void updateColor();

		void teXToolTipReady( int annotID );

		void nextEditorTab();
		void prevEditorTab();

	public:
		 abstractTool( pdfScene *Scene, toolBox *ToolBar, QStackedWidget *EditArea  );
		 ~abstractTool();

		 QString getAuthor() const { return author; };
		 QColor getColor() const { return color; };
		 void setAuthor( QString Author ) { author = Author;};
		 QColor setColor( QColor col ) { color = col;};
		 
		 QString getToolName() { return toolName; };

		 /* Returns a pointer to a newly created annotation, if the annotation was recognized,
		  * otherwise returns NULL. Note that the new annotation should have its position 
		  * relative to the page. However this should be intuitive :-) */
		 virtual abstractAnnotation *processAnnotation( PoDoFo::PdfDocument *doc, PoDoFo::PdfAnnotation *annotation, pdfCoords *transform ) = 0;

		 /* Called when the user wants to add a new annotation at scenePos 
		  * (scene coordinates) */
		 virtual void newActionEvent( const QPointF *scenePos ) = 0;


		 /* Edit the item (i.e. show an editing widget, etc.),
		  * if the item is currently already being edited, hide
		  * it instead */
		 virtual void editItem( abstractAnnotation *item );
		 
		 /* Finish editing the currently edited item (if any) */
		 virtual void finishEditing();

		 /* Called by the view when an event happens. Returns true, if
		  * tool handles the event, otherwise returns false; */
		 virtual bool handleEvent( viewEvent *ev );

		 /* Returns true, if the tool handles events for the item it */
		 virtual bool acceptEventsFor( QGraphicsItem *it ) = 0;

		 /* Returns the context menu for the item *it */
		 virtual QMenu *contextMenu( QGraphicsItem *it );

	public slots:
		 virtual void hideEditor();
		 
	signals:
		 void needKeyFocus( bool );

		 friend class abstractAnnotation;

};

class abstractAnnotation : public QGraphicsObject { 
  Q_OBJECT
	private:
		QPixmap toolTipPixMap, icon;
		QString toolTipRichText;
		bool waitingForToolTip, haveToolTip, moved, showingToolTip;
		enum toolTipType { pixmap, text };
		toolTipType tp;
		QString author,content;
		QDate date;
		QTime time;
		QColor color;

	protected:
	  	abstractTool *myTool;
		bool movable;
		virtual void setMyToolTip(const QPixmap &pixMap);
		virtual void setMyToolTip(const QString &richText);
		void setIcon( const QPixmap &icon);
		QPixmap getMyToolTipPixmap() const { return toolTipPixMap; };
		bool havePixmapTooltip() const { return (tp == pixmap); };
		

		void saveInfo2PDF( PoDoFo::PdfAnnotation *annot );
		abstractAnnotation( abstractTool *tool, PoDoFo::PdfAnnotation *annot, pdfCoords *transform );
	        friend class abstractTool;




	public:
		static bool isA( PoDoFo::PdfAnnotation *annotation ) { return false; };
 
	public:
		abstractAnnotation( abstractTool *tool );


		bool showToolTip( const QPoint &scPos );
		void hideToolTip();
		
		/* ??? if returns true, then it accepted the edit event ??? */
		virtual bool editSelf();
		
		virtual void finishEditing();
		 /* Returns the context menu for the item *it */
                virtual QMenu *contextMenu();

		bool hasToolTip();

		virtual bool isMovable() { return movable; };
		virtual QString getContent() const { return content; };
		void setContent( QString Content );
		void setAuthor( QString a ) { author = a; };
		void setDate( QDate d ) { date = d; };
		void setTime( QTime t ) { time = t; };
		void setColor( QColor col ) { if ( col.isValid() ) color = col; };
		QColor getColor() const { return color; };
		QString getAuthor() const { return author; };
		QDate getDate() const  { return date; };
		QTime getTime() const { return time; };

		virtual QRectF boundingRect() const;
		virtual void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget );



		virtual void saveToPdfPage( PoDoFo::PdfDocument *document, PoDoFo::PdfPage *pg, pdfCoords *coords ) = 0;
		
  signals:
    void needKeyFocus( bool );



};





#endif /* _abstractTool_H */
