#ifndef _config_H
#define _config_H

/***************************************************************
 * config.h
 * @Author:      Jonathan Verner (jonathan.verner@matfyz.cz)
 * @License:     GPL v2.0 or later
 * @Created:     2009-02-13.
 * @Last Change: 2009-02-13.
 * @Revision:    0.0
 * Description:
 * Usage:
 * TODO:
 *CHANGES:
 ***************************************************************/

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

		QString &operator[] (const QString key);
		
};

configurator& config();


#endif /* _config_H */
