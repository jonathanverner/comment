#ifndef linkLayer_p_H
#define linkLayer_p_H

#include <QtCore/QRectF>
#include <QtCore/QString>

#include <QtGui/QGraphicsObject>

class targetItem : public QGraphicsObject {
  Q_OBJECT
  private:
    QRectF area;
    QString name;
    
  public:
    targetItem( const QRectF &Area, const QString &Name = "" ): area(Area), name(Name) {};
    QRectF boundingRect() const { return area; };
    
  public slots:
    
    void activate();
    
  signals:
    
    void activated();
};


class linkItem : public QGraphicsObject {
  Q_OBJECT

  private:
    QRectF area;
    targetItem *tgt;

  public:
    linkItem( const QRectF &Area, targetItem *Tgt = NULL );
    QRectF boundingRect() const { return area; };
    
  public slots:
    void activate();
};


#endif // linkLayer_p_H