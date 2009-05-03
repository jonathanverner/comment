#ifndef LINKTOOL_H
#define LINKTOOL_H

#include "abstractTool.h"

#include <QtCore/QString>

class toolBox;
class linkAnnotation;

namespace PoDoFo {
    class PdfDestination;
}



class linkTool : public abstractTool {
    Q_OBJECT
private:

    struct destination {
        int page;
        QString name,type;
        QRectF viewPort;
        double value;
    };

    QList<destination> targets;
    QString addDestination( int page, QRectF viewPort, QString name = "" );
    QString addDestination( PoDoFo::PdfDestination &dest );
    static QPixmap *icon;
    static int nameCounter;

public:
    linkTool( pdfScene *Scene, toolBox *ToolBar, QStackedWidget *EditArea);
    virtual abstractAnnotation *processAnnotation( PoDoFo::PdfAnnotation *annotation, pdfCoords *transform );
    virtual void newActionEvent( const QPointF *ScenePos );
    virtual bool acceptEventsFor( QGraphicsItem *item );
    friend class linkAnnotation;
};

class linkAnnotation : public abstractAnnotation {
    public:
        linkAnnotation( linkTool *tool, PoDoFo::PdfAnnotation *linkAnnot = NULL, pdfCoords *transform = NULL );
        static bool isA( PoDoFo::PdfAnnotation *annotation );
        virtual void saveToPdfPage( PoDoFo::PdfDocument *document, PoDoFo::PdfPage *pg, pdfCoords *coords );
};



#endif // LINKTOOL_H
