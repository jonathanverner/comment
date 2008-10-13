#ifndef _abstractPageItem_H
#define _abstractPageItem_H

/***************************************************************
 * abstractPageItem.h
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
#include <QtCore/QString>
#include <QtCore/QDate>
#include <QtCore/QTime>
#include <QtGui/QGraphicsItem>
#include <QtGui/QPixmap>

#include <podofo/podofo.h>


class QGraphicsSceneMouseEvent;
class QGraphicsSceneHoverEvent;

class abstractPageItem;

class abstractItemEdit : public QObject {
	public:
		virtual void editItem( abstractPageItem *item ) = 0;
};



class abstractPageItem : public QGraphicsItem { 
	protected:
		abstractItemEdit *editForm;

		void setMyToolTip(const QPixmap &pixMap);
		void setMyToolTip(const QString &richText);

		void mouseMoveEvent( QGraphicsSceneMouseEvent *event );
		void mousePressEvent( QGraphicsSceneMouseEvent *event );
		void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );
		void hoverEnterEvent( QGraphicsSceneHoverEvent *event );
		void hoverLeaveEvent( QGraphicsSceneHoverEvent *event );

		
	private:
		QPixmap toolTipPixMap;
		QString toolTipRichText;
		bool waitingForToolTip, haveToolTip, moved, showingToolTip;
		enum toolTipType { pixmap, text };
		toolTipType tp;


		QString author;
		QDate date;
		QTime time;



	public:
		abstractPageItem(qreal x, qreal y): date( QDate::currentDate() ), time( QTime::currentTime() ), editForm(NULL) {setPos( x, y );};
		void setAuthor( QString a ) { author = a; };
		void setDate( QDate d ) { date = d; };
		void setTime( QTime t ) { time = t; };
		QString getAuthor() { return author; };
		QDate getDate() { return date; };
		QTime getTime() { return time; };

		virtual void saveToPdfPage( PoDoFo::PdfDocument *document, int page ) = 0;
};


#endif /* _abstractPageItem_H */
