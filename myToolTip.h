#ifndef _myToolTip_H
#define _myToolTip_H

/***************************************************************
 * myToolTip.h
 * @Author:      Jonathan Verner (jonathan.verner@matfyz.cz)
 * @License:     GPL v2.0 or later
 * @Created:     2008-10-13.
 * @Last Change: 2008-10-13.
 * @Revision:    0.0
 * Description:
 * Usage:
 * TODO:
 *CHANGES:
 ***************************************************************/

#include <QtGui/QPixmap>
#include <QtCore/QPoint>

class QLabel;
class QTimer;
class shower : public QObject {
  Q_OBJECT
	public slots:
		void show();
};


class myToolTip : public QObject {
	private:
		static void init();
		static bool waitingForDelay;

		static shower *sh;



	protected:
		static QLabel *toolTip;
		static void wait();
	public:
		static void showText( const QPoint &pos, const QString &text, bool delayed = true );
		static void update( const QString &text );
		static void update( const QPixmap &pix );

		static void showPixmap( const QPoint &pos, const QPixmap &pix, bool delayed = true );
		static void hide();
		

	protected:
		static void show();
		friend void shower::show();
};


#endif /* _myToolTip_H */
