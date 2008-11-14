#ifndef _textTool_H
#define _textTool_H

/***************************************************************
 * textTool.h
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

#include "abstractTool.h"

class toolBox;
class textAnnotation;

class textTool : public abstractTool { 
  Q_OBJECT
	private:
	  static QPixmap *icon;

	public:
		textTool( pdfScene *Scene, toolBox *ToolBar, QStackedWidget *EditArea);

		virtual abstractAnnotation *processAnnotation( PoDoFo::PdfAnnotation *annotation, pdfCoords *transform );
		virtual void newActionEvent( const QPointF *ScenePos );
		virtual bool acceptEventsFor( QGraphicsItem *item );
		friend class textAnnotation;
};

class textAnnotation : public abstractAnnotation { 
	public:
		textAnnotation( textTool *tool, PoDoFo::PdfAnnotation *textAnnot = NULL, pdfCoords *transform = NULL );
		static bool isA( PoDoFo::PdfAnnotation *annotation );
		virtual void saveToPdfPage( PoDoFo::PdfDocument *document, PoDoFo::PdfPage *pg, pdfCoords *coords );

};





#endif /* _textTool_H */
