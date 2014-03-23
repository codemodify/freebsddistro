


#ifndef __Architecture_Renderer_ArrowRenderer_H__
#define __Architecture_Renderer_ArrowRenderer_H__

// Qt headers
#include <QtGui/QGraphicsItem>

// Architector renderer
#include <Architecture.Renderer.IArrowRenderer.h>


class ArrowRenderer : public IArrowRenderer, public QGraphicsItem
{
    public:
        ArrowRenderer();

    protected:
        QRectF boundingRect() const;
        void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget );
};


#endif
