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

class textTool : public abstractTool { 
  Q_OBJECT
	private:
	  static QPixmap *icon;

	public:
		textTool( pdfScene *Scene, toolBoxr *ToolBar, QStackedWidget *EditArea);
		~textTool();

		virtual bool processAnnotation( PoDoFo::PdfAnnotation *annotation, int page );
		virtual void newActionEvent( QPoint *ScenePos );

	protected slots:
		void updateComment();
};

class textAnnotation : public abstractAnnotation { 
	private:
		QString comment;
	public:
		textAnnotation( textTool *tool, int page = 0, PoDoFo::PdfAnnotation *comment = NULL );
		void setText(QString comment);
		static bool isA( PoDoFo::PdfAnnotation *annotation );
		virtual void saveToPdfPage( PoDoFo::PdfDocument *document, int page ) = 0;

};





#endif /* _textTool_H */
