#ifndef __Architecture_Renderer_ArrowDoubleRenderer_H__
#define __Architecture_Renderer_ArrowDoubleRenderer_H__

// Qt headers
#include <QtGui/QGraphicsItem>

// Architector renderer
#include <Architecture.Renderer.IArrowDoubleRenderer.h>


class ArrowDoubleRenderer : public IArrowDoubleRenderer, public QGraphicsItem
{
    public:
        ArrowDoubleRenderer();

    protected:
        QRectF boundingRect() const;
        void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget );
};


#endif
