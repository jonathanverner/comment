/***************************************************************
 * testAnnotRM.cpp
 * @Author:      Jonathan Verner (jonathan.verner@matfyz.cz)
 * @License:     GPL v2.0 or later
 * @Created:     2008-10-19.
 * @Last Change: 2008-10-19.
 * @Revision:    0.0
 * Description:
 * Usage:
 * TODO:
 *CHANGES:
 ***************************************************************/
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



 
