#ifndef _util_H
#define _util_H

/***************************************************************
 * util.h
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


#include <QtGui/QMenu>

void prependMenu( QMenu *sourceMenu, QMenu *targetMenu );
void deleteUpToSeparator( QMenu *menu );

#endif /* _util_H */
