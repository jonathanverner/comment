/***************************************************************
 * hilightTool.cpp
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
#include <QtCore/QDebug>
#include <QtGui/QIcon>

#include "hilightTool.h"
#include "pageView.h"
#include "textLayer.h"
#include "toolBox.h"
#include "pdfScene.h"
#include "pdfPageItem.h"

QPixmap *hilightTool::icon = NULL;

hilightTool::hilightTool( pdfScene *Scene, toolBox *ToolBar, QStackedWidget *EditArea):
	abstractTool( Scene, ToolBar, EditArea )
{
  setToolName( "Hilight Tool" );
  if ( ! icon ) icon = new QPixmap( "hilight.png" );
  toolBar->addTool( QIcon(*icon), this );
}

hilightTool::~hilightTool() { 
  toolBar->removeTool( this );
};

void hilightTool::newActionEvent( const QPointF *ScenePos ) {
  hilightAnnotation *hi = new hilightAnnotation(this);
  scene->placeAnnotation( hi, ScenePos );
  hi->setZValue( 9 );
  editItem( hi );
}

void hilightTool::updateCurrentAnnotation( QPointF ScenePos ) { 
  hilightAnnotation *annot = dynamic_cast<hilightAnnotation*>(currentEditItem); 
  Q_ASSERT( annot );
  QPointF from = annot->pos();
  QPointF to = annot->parentItem()->mapFromScene( ScenePos );
  textLayer *txt;
  foreach( QGraphicsItem *item, scene->items( ScenePos ) ) {
	  if ( dynamic_cast<pdfPageItem*>(item) ) { 
	    foreach( QGraphicsItem *i, item->children() ) { 
	      if ( txt = dynamic_cast<textLayer*>(i) ) {
		qDebug() << "annot->updateSelection( txt->select("<<from<<","<<to<<") );";
		annot->updateSelection( txt->select( from, to ) );
		return;
	      }

	    }
	  }
  }
  return;
}

bool hilightTool::handleEvent( viewEvent *ev ) { 
  hilightAnnotation *annot;
  if ( ev->type() == viewEvent::VE_MOUSE_PRESS && ( ev->btnCaused() == Qt::LeftButton ) ) {

    qDebug() << "Starting hilight at "<< ev->scenePos();
    QPointF pos = ev->scenePos();
    newActionEvent( &pos );
    return true;
  } else if ( ev->type() == viewEvent::VE_MOUSE_RELEASE && ( ev->btnCaused() == Qt::LeftButton ) ) { 
    if ( ev->isClick() || ! (annot=dynamic_cast<hilightAnnotation*>(currentEditItem)) ) 
      return false;
    qDebug() << "Finishing hilight at "<< ev->scenePos();
    updateCurrentAnnotation( ev->scenePos() );
    currentEditItem = NULL;
    return true;
  } else if ( ev->type() == viewEvent::VE_MOUSE_MOVE && (annot=dynamic_cast<hilightAnnotation*>(currentEditItem)) ) { 
    qDebug() << "Updating hilight at "<< ev->scenePos();
    updateCurrentAnnotation( ev->scenePos() );
    return true;
  }
}



void hilightAnnotation::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget ) {
  QPointF myPos = pos();
  foreach( QRectF box, hBoxes ) { 
    painter->fillRect( QRectF(box.x()-myPos.x(), box.y()-myPos.y(), box.width(), box.height()), QColor( 255, 255, 0, 156 ) );
  }
}

void hilightAnnotation::updateBBox() {
  prepareGeometryChange();
  update( bBox );
  if (hBoxes.size() < 1 ) bBox = QRectF(0,0,0,0);
  else { 
    QRectF b = hBoxes.front();
    qreal minY=b.y(), minX=b.x(), maxY=minY+b.height(), maxX=minX+b.width();
    foreach( QRectF box, hBoxes ) { 
      if ( box.y() < minY ) minY = box.y();
      if ( box.x() < minX ) minX = box.x();
      if ( box.y() + box.height() > maxY ) maxY = box.y() + box.height();
      if ( box.x() + box.width() > maxX ) maxX = box.x() + box.width();
    }
    bBox=QRectF(mapFromParent(QPointF(minX,minY)),mapFromParent(QPointF(maxY,maxX)));
    qDebug() << "BBox: " << bBox;
  }
};

#include "hilightTool.moc"
