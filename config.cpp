/***************************************************************
 * config.cpp
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

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>
#include <QtCore/QDebug>
 
#include "config.h"

QString configurator::fName = "";
QHash<QString,QString> configurator::cfg;
bool configurator::haveTeXAndFriends;

configurator& config() { 
  static configurator *conf = new configurator();
  return *conf;
}

bool configurator::haveKey( const QString key ) const {
  return cfg.contains( key.toLower() );
}

void configurator::removeKey( const QString key ) {
  cfg.remove( key.toLower() );
}

  

bool configurator::findTeX() {
  if ( cfg["tex"] != "" && QFile::exists( cfg["tex"] ) ) return true;
  if ( QFile::exists( "/usr/bin/pdflatex" ) ) {
    cfg["tex"] = "/usr/bin/pdflatex";
    return true;
  }
  return false;
}

bool configurator::findGhostScript() {
  if ( cfg["gs"] != "" && QFile::exists( cfg["gs"] ) ) return true;
  if ( QFile::exists( "/usr/bin/gs" ) ) {
    cfg["gs"] = "/usr/bin/gs";
    return true;
  }
  return false;
}

configurator::configurator() 
{
  fName = QDir::homePath()+"/.comment";
  load();
  haveTeXAndFriends = findTeX() && findGhostScript();
  qDebug() << "initializing configurator";
}

void configurator::load() {
  qDebug() << "Loading config from "<<fName << "...";
  QFile cfgFile( fName );
  if ( ! cfgFile.open( QIODevice::ReadOnly | QIODevice::Text ) )
    return;
  QTextStream in(&cfgFile);
  while( ! in.atEnd() ) { 
    QString line = in.readLine();
    QRegExp exp(" *([^#= ]*) *= *([^ ][^;]*).*");
    if ( ! exp.exactMatch( line ) ) continue;
    QStringList list = exp.capturedTexts();
    cfg[list[1].toLower()]=list[2];
  }
  cfgFile.close();
}

void configurator::save() {
  qDebug() << "Saving config to "<<fName << "...";
  QFile cfgFile( fName );
  if ( ! cfgFile.open( QIODevice::WriteOnly | QIODevice::Text ) ) 
    return;
  QTextStream out(&cfgFile);
  foreach( QString key, cfg.keys() ) {
    out << key.toLower() << " = " << cfg[key] << ";" << endl;
  }
  cfgFile.close();
}

QString &configurator::operator[] (const QString key) { 
  return cfg[key.toLower()];
}

