/***************************************************************
 * pdfUtil.cpp
 * @Author:      Jonathan Verner (jonathan.verner@matfyz.cz)
 * @License:     GPL v2.0 or later
 * @Created:     2008-11-07.
 * @Last Change: 2008-11-07.
 * @Revision:    0.0
 * Description:
 * Usage:
 * TODO:
 *CHANGES:
 ***************************************************************/

#include <QtCore/QByteArray>
#include "pdfUtil.h"

using namespace PoDoFo;

PdfString pdfUtil::qStringToPdf( QString str ) { 
  QByteArray tmp = str.toUtf8();
  return PdfString( (pdf_utf8*) tmp.constData() );
}

QString pdfUtil::pdfStringToQ( PdfString str ) { 
  std::string tmp = str.GetStringUtf8();
  return QString::fromUtf8( tmp.c_str() );
}

