#ifndef _propertyTab_H
#define _propertyTab_H

/***************************************************************
 * propertyTab.h
 * @Author:      Jonathan Verner (jonathan.verner@matfyz.cz)
 * @License:     GPL v2.0 or later
 * @Created:     2009-01-07.
 * @Last Change: 2009-01-07.
 * @Revision:    0.0
 * Description:
 * Usage:
 * TODO:
 *CHANGES:
 ***************************************************************/

#include <QtGui/QWidget>
#include <QtGui/QColor>

class QLineEdit;
class QPushButton;
class QVBoxLayout;

class propertyTab : public QWidget { 
  Q_OBJECT

	  QLineEdit *authorEdit;
	  QPushButton *colorPush;
	  QVBoxLayout *layout;

          QColor color;

	protected:
	  void addWidgetWithLabel( QString Label, QWidget *w );

	public:
	  propertyTab( QWidget *parent = 0 );

	  void setAuthor( QString Author );
	  void setColor( QColor color );

	  QString getAuthor();
	  QColor getColor();
  
};


#endif /* _propertyTab_H */
