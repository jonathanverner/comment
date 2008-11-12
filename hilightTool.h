#ifndef _hilightTool_H
#define _hilightTool_H

/***************************************************************
 * hilightTool.h
 * @Author:      Jonathan Verner (jonathan.verner@matfyz.cz)
 * @License:     GPL v2.0 or later
 * @Created:     2008-11-07.
 * @Last Change: 2008-11-07.
 * @Revision:    0.0
 * Description:
 * Usage:
 * TODO:
 *CHANGES:
 ***************************************************************/



#include <QtCore/QList>
#include "abstractTool.h"

class toolBox;
class hilightAnnotation;

class hilightTool : public abstractTool { 
  Q_OBJECT
	private:
	  static QPixmap *icon;

	protected:
	  void updateCurrentAnnotation( QPointF ScenePos );

	public:
		hilightTool( pdfScene *Scene, toolBox *ToolBar, QStackedWidget *EditArea);
	         ~hilightTool();

		virtual abstractAnnotation *processAnnotation( PoDoFo::PdfAnnotation *annotation, pdfCoords *transform ) { return NULL; };
		virtual void newActionEvent( const QPointF *scPos );
		virtual void editItem( abstractAnnotation *item ) { currentEditItem = item;};
		virtual bool acceptEventsFor( QGraphicsItem *item );
		virtual bool handleEvent( viewEvent *ev );

		friend class hilightAnnotation;

};

class hilightAnnotation : public abstractAnnotation { 
	private:
		QList<QRectF> hBoxes;
		QRectF bBox;
		QPainterPath exactShape;
	public:
		hilightAnnotation( hilightTool *tool, PoDoFo::PdfAnnotation *hilightAnnot = NULL, pdfCoords *transform = NULL ): abstractAnnotation(tool), bBox(0,0,0,0) {movable=false;};
		~hilightAnnotation() {};
		

		void updateSelection( QList<QRectF> newSelection );

		void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget );
		QRectF boundingRect() const {return bBox;};
		QPainterPath shape() const;

		static bool isA( PoDoFo::PdfAnnotation *annotation ) { return false;}
		virtual void saveToPdfPage( PoDoFo::PdfDocument *document, PoDoFo::PdfPage *pg, pdfCoords *coords ) { return; };

		friend class hilightTool;

};



#endif /* _hilightTool_H */
