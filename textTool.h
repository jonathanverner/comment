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
		~textTool();

		virtual abstractAnnotation *processAnnotation( PoDoFo::PdfAnnotation *annotation );
		virtual void newActionEvent( const QPointF *ScenePos );
		virtual void editItem( abstractAnnotation *item );
		friend class textAnnotation;

	protected slots:
		void updateComment();
};

class textAnnotation : public abstractAnnotation { 
	private:
		QString comment;
	public:
		textAnnotation( textTool *tool, PoDoFo::PdfAnnotation *textAnnot = NULL );
		~textAnnotation();

		void setText(QString comment);
		QString getText() { return comment;}
		static bool isA( PoDoFo::PdfAnnotation *annotation );
		virtual void saveToPdfPage( PoDoFo::PdfDocument *document, int page ) {};

};





#endif /* _textTool_H */
