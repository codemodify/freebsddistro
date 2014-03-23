#ifndef __Architecture_Renderer_InterfaceRenderer_H__
#define __Architecture_Renderer_InterfaceRenderer_H__

// Qt headers
#include <QtGui/QGraphicsItem>

// Architector renderer
#include <Architecture.Renderer.IInterfaceRenderer.h>


class InterfaceRenderer : public IInterfaceRenderer, public QGraphicsItem
{
    public:
        InterfaceRenderer();

    protected:
        QRectF boundingRect() const;
        void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget );
};


#endif
