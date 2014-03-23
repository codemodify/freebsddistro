
// local headers
#include "Architecture.Renderer.AbstractClassRenderer.h"


AbstractClassRenderer::AbstractClassRenderer() :
    IAbstractClassRenderer(),
    QGraphicsItem()
{}

QRectF AbstractClassRenderer::boundingRect() const
{}

void AbstractClassRenderer::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
    // FIXME:....
}
