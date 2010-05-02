#ifndef _linkLayer_H
#define _linkLayer_H

/***************************************************************
 * linkLayer.h
 * @Author:      Jonathan Verner (jonathan.verner@matfyz.cz)
 * @License:     GPL v2.0 or later
 ***************************************************************/

#include "sceneLayer.h"

#include <QtGui/QGraphicsItem>
#include <QtCore/QRectF>
#include <QtCore/QHash>
#include <QtCore/QString>

class pdfScene;

class QSignalMapper;

class targetItem : public QGraphicsObject {
  Q_OBJECT
  private:
    QRectF area;
    QString name;
    
  public:
    targetItem( const QRectF &Area, const QString &Name = "" ): area(Area), name(Name) {};
    QRectF boundingRect() const { return area; };
    
    virtual void paint( QPainter*, const QStyleOptionGraphicsItem*, QWidget* ) {};
    
  public slots:
    
    void activate();
    
  signals:
    
    void activated();
};

class linkLayer : public sceneLayer {
  
  Q_OBJECT

  private:
    QSignalMapper *mapper;
    QHash<QString, targetItem *> targets;
    
  private slots:
    
    void emitGOTO( const QString &name );

  public:
    
    linkLayer( pdfScene* sc );

    targetItem *addTarget( const QString &name, const QRectF &target );
    targetItem *addTarget( const QString &name, const int page, const QRectF &target );
    
    void removeTarget( const QString &name );
    
  signals:
    
    void gotoTarget( QGraphicsItem *target );

};


#endif //_linkLayer_H