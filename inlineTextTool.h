

#ifndef INLINETEXTTOOL_H
#define INLINETEXTTOOL_H

#include "abstractTool.h"
#include <QtGui/QGraphicsTextItem>

class toolBox;
class inlineTextAnnotation;


class inlineTextTool : public abstractTool {
  Q_OBJECT
	private:
	  static QPixmap *icon;

	protected:
	  renderTeX *inlineRenderer;
	  QVector<inlineTextAnnotation*> int2annot;
	  
          void editAnnotationExtent( abstractAnnotation *item );
	  void editAnnotationText();
	  void finishEditing();
	  void prepareTeX( inlineTextAnnotation *item );
	  
  protected slots:
    
    void teXReady( int item );

	public:
                inlineTextTool(pdfScene* Scene, toolBox* ToolBar, QStackedWidget* EditArea);
	        ~inlineTextTool();
		
		virtual abstractAnnotation *processAnnotation( PoDoFo::PdfAnnotation *annotation, pdfCoords *transform );
		virtual void newActionEvent( const QPointF *scPos );
		virtual bool acceptEventsFor( QGraphicsItem *item );
		/*virtual bool handleEvent( viewEvent *ev );*/
		virtual void editItem( abstractAnnotation *item );

		friend class inlineTextAnnotation;

};
  
class inlineTextAnnotation : public abstractAnnotation {
  Q_OBJECT
  static int inlineAnnotationCount;
  private:
    QGraphicsTextItem *item;
    int inlineID;
    bool teXAppearance;
  QRectF brec;
    
  protected:
    void setTeXAppearance(bool);
   
    
	  
	public:
		inlineTextAnnotation( inlineTextTool *tool, PoDoFo::PdfAnnotation *hilightAnnot = NULL, pdfCoords *transform = NULL );
		~inlineTextAnnotation();


		void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget );
		QRectF boundingRect() const;
		QPainterPath shape() const;
		

		static bool isA( PoDoFo::PdfAnnotation *annotation );
		virtual void saveToPdfPage( PoDoFo::PdfDocument *document, PoDoFo::PdfPage *pg, pdfCoords *coords );


		friend class inlineTextTool;

};

#endif // INLINETEXTTOOL_H
