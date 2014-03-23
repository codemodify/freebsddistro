#ifndef __Architecture_Renderer_AbstractClassRenderer_H__
#define __Architecture_Renderer_AbstractClassRenderer_H__

// Qt headers
#include <QtGui/QGraphicsItem>

// Architector renderer
#include <Architecture.Renderer.IAbstractClassRenderer.h>


class AbstractClassRenderer : public IAbstractClassRenderer, public QGraphicsItem
{
    public:
        AbstractClassRenderer();

    protected:
        QRectF boundingRect() const;
        void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget );
};


#endif
