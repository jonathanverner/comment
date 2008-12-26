#ifndef _testTeXRender_H
#define _testTeXRender_H

/***************************************************************
 * testTeXRender.h
 * @Author:      Jonathan Verner (jonathan.verner@matfyz.cz)
 * @License:     GPL v2.0 or later
 * @Created:     2008-11-29.
 * @Last Change: 2008-11-29.
 * @Revision:    0.0
 * Description:
 * Usage:
 * TODO:
 *CHANGES:
 ***************************************************************/

#include <QtGui/QTextEdit>
#include <QtGui/QLabel>

#include "renderTeX.h"

class renderer : public QObject { 
  Q_OBJECT
	private:
	  QLabel *lbl;
	  QTextEdit *txt;
	  renderTeX r;
	  int itemID;

	public:
	  renderer( QLabel *label, QTextEdit *text );

	public slots:
	  void updateTeX();

	protected slots:
	  void pdfReady( int i );
};

#endif /* _testTeXRender_H */
