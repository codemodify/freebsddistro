#ifndef __Architecture_Renderer_PropertiesRenderer_H__
#define __Architecture_Renderer_PropertiesRenderer_H__

// Qt headers
#include <QtGui/QGraphicsItem>

// Architector renderer
#include <Architecture.Renderer.IPropertiesRenderer.h>


class PropertiesRenderer : public IPropertiesRenderer, public QGraphicsItem
{
    public:
        PropertiesRenderer();

    protected:
        QRectF boundingRect() const;
        void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget );
};


#endif
