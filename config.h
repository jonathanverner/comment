#ifndef _config_H
#define _config_H

/**  This file is part of comment
*
*  File: config.h
*  Created: 13. 02. 2009
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

#include <QtCore/QString>
#include <QtCore/QHash>


class configurator { 
	private:

		static QString fName;
		static QHash<QString, QString> cfg;

		static bool haveTeXAndFriends;


		bool findTeX();
		bool findGhostScript();

	public:
		configurator();

		void load();
		void save();
		
		bool haveTeX() { return haveTeXAndFriends; }

		bool haveKey(const QString key) const;
		void removeKey(const QString key );

		QString &operator[] (const QString key);
		
};

configurator& config();


#endif /* _config_H */
