#ifndef _searchBar_H
#define _searchBar_H

/***************************************************************
 * searchBar.h
 * @Author:      Jonathan Verner (jonathan.verner@matfyz.cz)
 * @License:     GPL v2.0 or later
 * @Created:     2009-01-31.
 * @Last Change: 2009-01-31.
 * @Revision:    0.0
 * Description:
 * Usage:
 * TODO:
 *CHANGES:
 ***************************************************************/

#include <QtCore/QString>
#include <QtGui/QWidget>

class QLineEdit;
class QPushButton;

class searchBar : public QWidget { 
  Q_OBJECT
	private:
	  QLineEdit *edit;
	  QPushButton *next,*prev;

	public:
		searchBar( QWidget *parent );

	public slots:


		void setText( QString text );
		void setFound();
		void setMissed();
		void setNone();
		void setNumOfMatches(int n);
		void focus();

	signals:

		void textChanged( QString text );
		void nextMatch();
		void prevMatch();

};


#endif /* _searchBar_H */
