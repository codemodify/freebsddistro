#ifndef __Architecture_Renderer_ClassRenderer_H__
#define __Architecture_Renderer_ClassRenderer_H__

// Qt headers
#include <QtGui/QGraphicsItem>

// Architector renderer
#include <Architecture.Renderer.IClassRenderer.h>


class ClassRenderer : public IClassRenderer, public QGraphicsItem
{
    public:
        ClassRenderer();

    protected:
        QRectF boundingRect() const;
        void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget );
};


#endif
