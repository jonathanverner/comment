#ifndef _linkLayer_H
#define _linkLayer_H

/***************************************************************
 * linkLayer.h
 * @Author:      Jonathan Verner (jonathan.verner@matfyz.cz)
 * @License:     GPL v2.0 or later
 ***************************************************************/

#include "sceneLayer.h"

#include <QtGui/QGraphicsObject>
#include <QtCore/QRectF>
#include <QtCore/QHash>

class pdfScene;
class QMenu;
class QSignalMapper;

namespace PoDoFo {
  class PdfDocument;
  class PdfOutlineItem;
}

class linkItem : public QGraphicsObject {
  Q_OBJECT
    
  private:
    QRectF area;
    QString name;
    linkItem *target, *parent;
    
  public:
    linkItem( const QRectF &Area, const QString &Name = "" ): area(Area), name(Name), target(NULL), parent(NULL) {};
    
    void setTarget( linkItem *tgt ) { target = tgt; };
    void setParent( linkItem *pnt ) { parent = pnt; };
 
    QString getName() const { return name; };
    linkItem *getParent() const { return parent; };
    linkItem *getTarget() const { return target; };
    
    QRectF boundingRect() const { return area; };

    
  signals:
    
    void activated();
    
};

class linkLayer : public sceneLayer {

  private:
    QSignalMapper *mapper;
    QHash<QString, linkItem *> items;
    void loadToc( PoDoFo::PdfDocument* doc );
    linkItem *loadOutlineItem( PoDoFo::PdfOutlineItem *item, linkItem *parent, const QString &path );
    
  private slots:
    
    void emitGOTO( const QString &name );

  public:
    
    linkLayer( pdfScene *sc, PoDoFo::PdfDocument *doc );
    
    QMenu *tocMenu();
    
    linkItem *addLink( const QRectF &area, const QString &target );
    linkItem *addTOCItem( const QString &name, linkItem *parent, const QString &target );
    linkItem *addTOCItem( const QString &name, linkItem *parent, linkItem *target );
    linkItem *addTarget( const QString &name, const QRectF &target );
    
    linkItem *getTarget( const QString &name );
    
    void removeTarget( const QString &name );
    void removeLink( linkItem *item );
    
  signals:
    
    void gotoTarget( QGraphicsItem *target );
    void tocChanged();

};


#endif //_linkLayer_H