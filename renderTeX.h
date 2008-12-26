#ifndef _renderTeX_H
#define _renderTeX_H

/***************************************************************
 * renderTeX.h
 * @Author:      Jonathan Verner (jonathan.verner@matfyz.cz)
 * @License:     GPL v2.0 or later
 * @Created:     2008-11-17.
 * @Last Change: 2008-11-17.
 * @Revision:    0.0
 * Description:
 * Usage:
 * TODO:
 *CHANGES:
 ***************************************************************/
#include <QtCore/QVector>
#include <QtCore/QStack>
#include <QtCore/QCache>
#include <QtCore/QString>
#include <QtGui/QPixmap>

class renderItem;

class renderTeX : public QObject { 
  Q_OBJECT
	private:
		struct cachedPage { 
		  bool format_inline;
		  qreal zoom;
		  QPixmap pix;
		};
		static QCache<int, struct cachedPage> renderCache;
		QVector<renderItem*> items;
		QStack<int> available_ids;

		QString preambule;

	protected slots:
		void renderingFinished( int i );

	public:
		renderTeX( QString preamb="" );

		void setPaths( QString pdfLaTeX, QString ghostScript );
		void setPreambule( QString preambule );

		/* This method should always succeed */
		int addItem( QString source, QString preambule = "" );
		void updateItem( int item, QString source, QString preambule = "" );
		void deleteItem( int item );
		// sizeHint is the wanted width in millimeters
		QPixmap render( int item, bool format_inline = false, qreal zoom = 1, int sizeHint = 50 );
		void preRender( int item, bool format_inline = false, int sizeHint = 50 );
	signals:
		void itemReady( int item );


		
};


#endif /* _renderTeX_H */
