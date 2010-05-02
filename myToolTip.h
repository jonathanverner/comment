#ifndef _myToolTip_H
#define _myToolTip_H

/**  This file is part of comment
*
*  File: myToolTip.h
*  Created: 13. 10. 2008
*  Author: Jonathan Verner <jonathan.verner@matfyz.cz>
*  License: GPL v2 or later
*
*  Copyright (C) 2010 Jonathan Verner <jonathan.verner@matfyz.cz>
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Library General Public
*  License as published by the Free Software Foundation; either
*  version 2 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Library General Public License for more details.
*
*  You should have received a copy of the GNU Library General Public License
*  along with this library; see the file COPYING.LIB.  If not, write to
*  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
*  Boston, MA 02110-1301, USA.
*/

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
