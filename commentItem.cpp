/***************************************************************
 * commentItem.cpp
 * @Author:      Jonathan Verner (jonathan.verner@matfyz.cz)
 * @License:     GPL v2.0 or later
 * @Created:     2008-10-13.
 * @Last Change: 2008-10-13.
 * @Revision:    0.0
 * Description:
 * Usage:
 * TODO:
 *CHANGES:
 ***************************************************************/
#include <QtGui/QPainter>
#include <QtGui/QStyleOptionGraphicsItem>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QTextEdit>
#include <QtCore/QDebug>
#include "commentItem.h"
#include "myToolTip.h"



QTextEdit *commentItemEdit::editor = NULL;
commentItem *commentItemEdit::currentItem = NULL;
dispatcher *commentItemEdit::dsp = NULL;

void dispatcher::dispatch() { 
  commentItemEdit::update();
}

void commentItemEdit::editItem( abstractPageItem *item ) { 
  qDebug() << "Editing item";
  if ( ! editor ) init();
  if ( ! editor->isVisible() ) { 
    currentItem = dynamic_cast<commentItem *>(item);
    editor->show();
  } else editor->hide();
}

void commentItemEdit::init() { 
  if ( ! editor ) {
    editor = new QTextEdit();
    editor->acceptRichText();
    dsp = new dispatcher();
    connect( editor, SIGNAL(textChanged()), dsp, SLOT( dispatch() ) );
  }
}

void commentItemEdit::update() { 
  if ( ! currentItem ) return;
  else currentItem->setText( editor->toHtml() );
}

commentItem::commentItem(qreal x, qreal y): abstractPageItem(x,y), comment(""), icon(0,0)
{
    editor = new commentItemEdit();
    editor->init();
    editForm = dynamic_cast<abstractItemEdit *>(editor); 
}


commentItem::~commentItem()
{
}

void commentItem::setIcon(QPixmap &icn) {
  if ( ! ( icn.rect() == icon.rect() ) ) prepareGeometryChange();
  icon = icn;
  update( icon.rect() );
}

void commentItem::setText(QString cmnt) {
  comment=cmnt;
  setMyToolTip( comment );
}

QRectF commentItem::boundingRect() const {
  return (QRectF) icon.rect();
}

void commentItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget ) {
  painter->drawPixmap( option->exposedRect, icon, option->exposedRect );

}

void commentItem::saveToPdfPage( PoDoFo::PdfDocument *document, int page ) {

}

/*void commentItem::mousePressEvent( QGraphicsSceneMouseEvent *event ) { */
  /*event->accept();*/
  /*qDebug() << "Mouse pressed";*/
/*}*/

/*void commentItem::mouseMoveEvent( QGraphicsSceneMouseEvent *event ) { */
  /*qreal dx = event->pos().x() - event->lastPos().x();*/
  /*qreal dy = event->pos().y() - event->lastPos().y();*/
  /*setPos( pos().x() + dx, pos().y() + dy );*/
  /*qDebug() << "Mouse moved";*/
/*}*/


/*void commentItem::hoverEnterEvent( QGraphicsSceneHoverEvent *event ) { */
  /*qDebug() << "Mouse hover ENTER";*/
  /*myToolTip::showPixmap(event->screenPos(), hoverPixmap );*/
/*}*/

/*void commentItem::hoverLeaveEvent( QGraphicsSceneHoverEvent *event ) { */
  /*qDebug() << "Mouse hover LEAVE";*/
  /*myToolTip::hide();*/
/*}*/

#include "commentItem.moc"
