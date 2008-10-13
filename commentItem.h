#ifndef _commentItem_H
#define _commentItem_H

/***************************************************************
 * commentItem.h
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
#include <QtCore/QRectF>
#include <QtGui/QPixmap>
#include <podofo/podofo.h>

#include "abstractPageItem.h"

class QPainter;
class QStyleOptionGraphicsItem;
class QGraphicsSceneMouseEvent;

class commentItem;
class QTextEdit;
class dispatcher : public QObject { 
  Q_OBJECT
	public slots:
	  void dispatch();
};



class commentItemEdit : public abstractItemEdit {
	private:
		static commentItem *currentItem;
		static QTextEdit *editor;
		static dispatcher *dsp;

	public:
		virtual void editItem( abstractPageItem *item );
		static void init();

	protected:
		static void update();
		friend void dispatcher::dispatch();

};

class commentItem : public abstractPageItem { 
	private:
		QString comment;
		QPixmap icon;
		commentItemEdit *editor;
	public:
		commentItem(qreal x, qreal y);
		~commentItem();

		void setIcon(QPixmap &icon);
		void setText(QString comment);

		QRectF boundingRect() const;
		void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget );
		void saveToPdfPage( PoDoFo::PdfDocument *document, int page );

};


#endif /* _commentItem_H */
