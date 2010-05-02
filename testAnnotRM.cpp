/**  This file is part of project comment
 *
 *  File: testAnnotRM.cpp
 *  Created: 2008-10-19
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

#include <QtCore/QDebug>
#include <podofo/podofo.h>

using namespace PoDoFo;
int main( int argc, char **argv ) { 
  if ( argc != 3 ) {
    qDebug() << "Wrong number of arguments.";
    qDebug() << "Usage: "<<argv[0]<<" pdfIN pdfOUT";
    exit(-1);
  }
  PdfMemDocument pdf( argv[1] );

/*  PdfPage *pgA = pdf.GetPage(0);
  pgA->DeleteAnnotation(0);
  pdf.Write(argv[2]);
  return 0; */

  PdfPage *pg;
  int max;
  for(int i=0; i< pdf.GetPageCount(); i++ ) {
    pg = pdf.GetPage(i);
/*    max=pg->GetNumAnnots();
    for(int e=0; e<max; e++ ) { 
      pg->DeleteAnnotation( e );
    }*/
    try { 
      while( pg->GetNumAnnots() > 0 ) { 
	pg->DeleteAnnotation(0);
      }
    } catch ( PdfError error ) { 
      qDebug() << error.what();
    }
  }
  pdf.Write(argv[2]);
  return 0;
}



 
