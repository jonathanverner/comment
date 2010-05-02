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
#include <QtCore/QString>

class pdfScene;

class targetItem;
class linkItem;

class QMenu;
class QSignalMapper;

namespace PoDoFo {
  class PdfDocument;
  class PdfOutlineItem;
};



class linkLayer : public sceneLayer {

  private:
    QSignalMapper *mapper;
    QHash<QString, targetItem *> targets;
    QList<linkItem *> links;
    
  private slots:
    
    void emitGOTO( const QString &name );

  public:
    
    linkLayer( pdfScene *sc, PoDoFo::PdfDocument *doc );

    
    void addLink( const QRectF &area, const QString &target );
    void addTarget( const QString &name, const QRectF &target );
    
    void removeTarget( const QString &name );
    void removeLink( linkItem *item );
    
  signals:
    
    void gotoTarget( QGraphicsItem *target );

};


#endif //_linkLayer_H