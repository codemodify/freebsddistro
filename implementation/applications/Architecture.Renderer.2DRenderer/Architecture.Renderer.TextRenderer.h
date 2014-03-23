#ifndef __Architecture_Renderer_TextRenderer_H__
#define __Architecture_Renderer_TextRenderer_H__

// Qt headers
#include <QtGui/QGraphicsItem>

// Architector renderer
#include <Architecture.Renderer.ITextRenderer.h>


class TextRenderer : public ITextRenderer, public QGraphicsItem
{
    public:
        TextRenderer();

    protected:
        QRectF boundingRect() const;
        void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget );
};


#endif
