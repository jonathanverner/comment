/**  This file is part of project comment
 *
 *  File: config.cpp
 *  Created: 2009-02-13
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

 
#include "config.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>
#include <QtCore/QDebug>

QString configurator::fName = "";
QHash<QString,QString> configurator::cfg;
bool configurator::haveTeXAndFriends;

configurator& config() { 
  static configurator *conf = new configurator();
  return *conf;
}

bool configurator::haveKey( const QString key ) const {
  return cfg.contains( key.toLower().replace(' ','_') );
}

void configurator::removeKey( const QString key ) {
  cfg.remove( key.toLower().replace(' ','_') );
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
    QRegExp exp(" *([^ ][^#= ]*) *= *([^ ][^;]*).*");
    if ( ! exp.exactMatch( line ) ) continue;
    QStringList list = exp.capturedTexts();
    cfg[list[1].toLower().replace(' ','_')]=list[2];
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
    out << key.toLower().replace(' ','_') << " = " << cfg[key] << ";" << endl;
  }
  cfgFile.close();
}

QString &configurator::operator[] (const QString key) { 
  return cfg[key.toLower().replace(' ','_')];
}

