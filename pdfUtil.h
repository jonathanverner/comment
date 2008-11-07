#ifndef _pdfUtil_H
#define _pdfUtil_H

/***************************************************************
 * pdfUtil.h
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

#include <QtCore/QString>
#include <podofo/podofo.h>

namespace pdfUtil { 

  PoDoFo::PdfString qStringToPdf( QString str );
  QString pdfStringToQ( PoDoFo::PdfString str );

}

#endif /* _pdfUtil_H */
