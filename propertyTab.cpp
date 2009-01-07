/***************************************************************
 * propertyTab.cpp
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


#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>


#include "propertyTab.h"

propertyTab::propertyTab( QWidget *parent) : QWidget( parent ) {
  layout = new QVBoxLayout;
  authorEdit = new QLineEdit;
  colorPush = new QPushButton;
  addWidgetWithLabel( "Author", authorEdit );
  addWidgetWithLabel( "Color", colorPush );
  setLayout( layout );
}

void propertyTab::addWidgetWithLabel( QString Label, QWidget *w ) { 
  QHBoxLayout *tempLayout = new QHBoxLayout;
  tempLayout->addWidget( new QLabel( Label ) );
  tempLayout->addWidget( w );
  layout->addLayout( tempLayout );
}

void propertyTab::setAuthor( QString Author ) {
  authorEdit->setText( Author );
}

void propertyTab::setColor( QColor col ) {
  color = col;
}

QString propertyTab::getAuthor() {
  return authorEdit->text();
}

QColor propertyTab::getColor() {
  return color;
}

#include "propertyTab.moc"
