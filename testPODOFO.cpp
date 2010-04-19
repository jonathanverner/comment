/***************************************************************
 * testPODOFO.cpp
 * @Author:      Jonathan Verner (jonathan.verner@matfyz.cz)
 * @License:     GPL v2.0 or later
 * @Created:     2008-10-17.
 * @Last Change: 2008-10-17.
 * @Revision:    0.0
 * Description:
 * Usage:
 * TODO:
 *CHANGES:
 ***************************************************************/
#include <podofo/podofo.h>
#include <QtCore/QDebug>

using namespace PoDoFo;



int main( int argc, char **argv ) { 
  if ( argc < 3 ) {
    qDebug() << "Usage: "<< argv[0] << " inPdf annotation_pdf ";
    return -1;
  }
  PdfMemDocument pdfDoc( argv[1] );
  PdfMemDocument app( argv[2] );
  PdfMemDocument icn( "tst.pdf" );
  PdfXObject *annotAppearance = new PdfXObject( app, 0, &pdfDoc );
  PdfXObject *annIcon = new PdfXObject( icn, 0, &pdfDoc );
  PdfPage *firstPage = pdfDoc.GetPage( 0 );
  double w = annotAppearance->GetPageSize().GetWidth(), h = annotAppearance->GetPageSize().GetHeight();
  double iw= annIcon->GetPageSize().GetWidth(), ih = annIcon->GetPageSize().GetHeight();
  qDebug() << "(100,"<<400+h-ih/4<<","<<iw/4<<","<<ih/4<<")";
  qDebug() << "Page Size Width x Heigth:"<< firstPage->GetPageSize().GetWidth() <<" x "<< firstPage->GetPageSize().GetHeight();
  PdfAnnotation *aIcon = firstPage->CreateAnnotation( ePdfAnnotation_FreeText, PdfRect(100,400+h-ih,iw,ih) );
//  PdfAnnotation *annot = firstPage->CreateAnnotation( ePdfAnnotation_Text, PdfRect(100,400+h-ih/4,iw/4,ih/4) );
  PdfAnnotation *annot = firstPage->CreateAnnotation( ePdfAnnotation_Text,  PdfRect(100,400,w,h) );
  firstPage = pdfDoc.GetPage( 1 );
  PdfAnnotation *tst = firstPage->CreateAnnotation( ePdfAnnotation_Text, PdfRect(200,400,20,20) );
  tst = firstPage->CreateAnnotation( ePdfAnnotation_FreeText, PdfRect(200,400,20,20) );
  tst->SetAppearanceStream( annIcon );
  tst->SetTitle("Jonathan Verner");
  tst->SetContents( PdfString("Joniho testovaci anotace") );
  PdfXObject *obj = new PdfXObject( PdfRect(0,0,w,h), &pdfDoc ); 
  annot->SetAppearanceStream( obj ); //, annotAppearance );
  aIcon->SetTitle("Testing annotation");
  aIcon->SetContents( PdfString("Contents of the annotation") );
  aIcon->SetOpen( true );
  aIcon->SetAppearanceStream( annIcon );
  pdfDoc.Write( "testPoDoFo.pdf" );
}
  

  
 
