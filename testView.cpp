/***************************************************************
 * testView.cpp
 * @Author:      Jonathan Verner (jonathan.verner@matfyz.cz)
 * @License:     GPL v2.0 or later
 * @Created:     2008-10-12.
 * @Last Change: 2008-10-12.
 * @Revision:    0.0
 * Description:
 * Usage:
 * TODO:
 *CHANGES:
 ***************************************************************/
 
#include "testView.h"

void testView::zoomIN() {
  scale ( 1.5, 1.5 );
}

void testView::zoomOUT() {
  scale( 0.7, 0.7 ); 
}

void testView::up() {
}

void testView::down() {
}

void testView::left() {
}

void testView::right() {
}

#include "testView.moc"
