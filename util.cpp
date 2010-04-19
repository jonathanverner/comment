#include "util.h"

void prependMenu(QMenu* sourceMenu, QMenu* targetMenu) {
  QAction *separator=NULL;
  foreach( QAction *act, targetMenu->actions() ) {
    if ( act->isSeparator() ) {
      separator = act;
      break;
    }
  }
  targetMenu->insertActions(separator,sourceMenu->actions());
}

void deleteUpToSeparator( QMenu* menu ) {
  foreach( QAction *act, menu->actions() ) {
    if ( ! act->isSeparator() ) menu->removeAction( act );
    else break;
  }
}

