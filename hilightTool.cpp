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

bool hilightTool::acceptEventsFor( QGraphicsItem *item ) {  
  /*bool ret = */ return dynamic_cast<hilightAnnotation*>( item );
/*  abstractAnnotation *a = dynamic_cast<abstractAnnotation*>( item );
  if ( a ) qDebug() << "Checking item type" << a->getToolName();
  if ( ret ) qDebug() << "Accepting event for item";
  return ret;*/
}

bool hilightTool::handleEvent( viewEvent *ev ) { 
  hilightAnnotation *annot;
  if ( ev->type() == viewEvent::VE_MOUSE_PRESS && ( ev->btnCaused() == Qt::LeftButton ) ) {
    if ( annot = dynamic_cast<hilightAnnotation*>(ev->item()) ) { 
      qDebug() << "Editing hilight at "<< ev->scenePos();
      editItem( annot );
    } else {
      ev->scene()->addRect(QRectF(ev->scenePos(), QSizeF(2,2)));
      qDebug() << "Starting hilight at "<< ev->scenePos();
      QPointF pos = ev->scenePos();
      newActionEvent( &pos );
    }
    return true;
  } else if ( ev->type() == viewEvent::VE_MOUSE_RELEASE && ( ev->btnCaused() == Qt::LeftButton ) ) { 
    if ( ev->isClick() || ! (annot=dynamic_cast<hilightAnnotation*>(currentEditItem)) ) return false;
    qDebug() << "Finishing hilight at "<< ev->scenePos();
    updateCurrentAnnotation( ev->scenePos() );
    annot->finalizeBBox();
    currentEditItem = NULL;
    return true;
  } else if ( ev->type() == viewEvent::VE_MOUSE_MOVE && (annot=dynamic_cast<hilightAnnotation*>(currentEditItem)) ) { 
    qDebug() << "Updating hilight at "<< ev->scenePos();
    updateCurrentAnnotation( ev->scenePos() );
    qDebug() << "Current BBox:" << annot->boundingRect();
    if ( ! (ev->btnState() & Qt::LeftButton) ) { // Missed a mouse release, end editing annotation
      qDebug() << "WARNING MISSED MOUSE RELEASE EVENT!!!";
      annot->finalizeBBox();
      currentEditItem=NULL;
    };
    return true;
  }
  return false;
}



void hilightAnnotation::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget ) {
  QPointF myPos = pos();
  foreach( QRectF box, hBoxes ) { 
    painter->fillRect( QRectF(box.x()-myPos.x(), box.y()-myPos.y(), box.width(), box.height()), QColor( 255, 255, 0, 156 ) );
  }
}

QPainterPath hilightAnnotation::shape() const { 
//  if (exShapeValid) return exactShape;
  QPainterPath tmp;
  tmp.addRect(boundingRect());
  return tmp;
}

void hilightAnnotation::finalizeBBox() { 
  QPainterPath tmp;
  update();
  prepareGeometryChange();
  if (hBoxes.size() < 1 ) bBox = QRectF(0,0,0,0);
  else { 
    QRectF b = hBoxes.front();
    qreal minY=b.y(), minX=b.x(), maxY=minY+b.height(), maxX=minX+b.width();
    foreach( QRectF box, hBoxes ) { 
      tmp.addRect(box);
      if ( box.y() < minY ) minY = box.y();
      if ( box.x() < minX ) minX = box.x();
      if ( box.y() + box.height() > maxY ) maxY = box.y() + box.height();
      if ( box.x() + box.width() > maxX ) maxX = box.x() + box.width();
    }
    bBox=QRectF(mapFromParent(QPointF(minX,minY)),mapFromParent(QPointF(maxY,maxX)));
//    qDebug() << "BBox: " << bBox;
  }
  exactShape=tmp;
  exShapeValid=true;
  update();
};

void hilightAnnotation::updateBBox() {
  update();
  prepareGeometryChange();
  exShapeValid=false;
    QRectF b = mapToParent(bBox).boundingRect();
    qreal minY=b.y(), minX=b.x(), maxY=minY+b.height(), maxX=minX+b.width();
    foreach( QRectF box, hBoxes ) { 
      if ( box.y() < minY ) minY = box.y();
      if ( box.x() < minX ) minX = box.x();
      if ( box.y() + box.height() > maxY ) maxY = box.y() + box.height();
      if ( box.x() + box.width() > maxX ) maxX = box.x() + box.width();
    bBox=QRectF(mapFromParent(QPointF(minX,minY)),mapFromParent(QPointF(maxY,maxX)));
//    qDebug() << "BBox: " << bBox;
  }
  update();
};

#include "hilightTool.moc"
