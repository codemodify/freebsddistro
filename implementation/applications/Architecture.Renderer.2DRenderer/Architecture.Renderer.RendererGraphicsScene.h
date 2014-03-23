#ifndef __Architecture_Renderer_ModelRendererGraphicsScene_H__
#define __Architecture_Renderer_ModelRendererGraphicsScene_H__

// Qt headers
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsItem>
#include <QtCore/QPointF>


class RendererGraphicsScene : public QGraphicsScene
{
    public:
        RendererGraphicsScene();
        ~RendererGraphicsScene();

        // overrides from QGraphicsScene
        protected:
            void mousePressEvent    ( QGraphicsSceneMouseEvent* mouseEvent  );
            void mouseMoveEvent     ( QGraphicsSceneMouseEvent* mouseEvent  );
            void mouseReleaseEvent  ( QGraphicsSceneMouseEvent* mouseEvent  );
            void drawBackground     ( QPainter* painter, const QRectF& rect );

        typedef struct 
        {
            qreal _x;
            qreal _y;
            qreal _w;
            qreal _h;
        } Size;
            
        private:
            Size            		_size;
            QPointF				_startPoint;
//            Shapes::IShape* 		_lastShape;
            QGraphicsItem*  		_lastShapeAsGraphicsItem;
//            Properties::Renderer*	_properties;
};

#endif
