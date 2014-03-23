
// Qt headers
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QPainter>

// local headers
#include "Architecture.Renderer.RendererGraphicsScene.h"


RendererGraphicsScene::RendererGraphicsScene() :
    QGraphicsScene()
{
    _size._x = -2000;
    _size._y = -3000;
    _size._w = 2000;
    _size._h = 3000;

    setSceneRect( _size._x, _size._y, _size._w, _size._h );
    setBackgroundBrush( Qt::gray );
    
//	_properties = new Properties::Renderer();
//	addItem( _properties );
}

RendererGraphicsScene::~RendererGraphicsScene()
{}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- overrides
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void RendererGraphicsScene::mousePressEvent( QGraphicsSceneMouseEvent* mouseEvent )
{
//	_startPoint = mouseEvent->scenePos();
//
//	// mouse mode, select items, do some operations on them
//	if( true == shapeButtons[Architecture::Shapes::eMouse]->isChecked() )
//	{
//    	_lastShapeAsGraphicsItem    = itemAt( mouseEvent->scenePos() );
//	    _lastShape                  = dynamic_cast< Shapes::IShape* >( _lastShapeAsGraphicsItem );
//	    
//		if( _lastShapeAsGraphicsItem != _properties )
//		{
//			_properties->setTarget( _lastShape );
//			_properties->setZValue( items().count() );
//		}
//	}
//
//	// shapes mode, add some shapes on the schene
//	else if( true == shapeButtons[Architecture::Shapes::eText]->isChecked() )
//	{
//	    QString shapeName = Architecture::Shapes::shapeNames[ Architecture::Shapes::eText ];
//	
//		_lastShape = Shapes::Factory().instanciate( shapeName );
//		_lastShape->setScenePosition( mouseEvent->scenePos() );
//		
//		addItem( _lastShapeAsGraphicsItem );
//	}
//
//	else if( true == shapeButtons[Architecture::Shapes::eArrow]->isChecked() )
//	{
//    	QString shapeName = Architecture::Shapes::shapeNames[ Architecture::Shapes::eText ];
//	
//		_lastShape = Shapes::Factory().instanciate( shapeName );
//		_lastShape->setScenePosition( mouseEvent->scenePos() );
//		
//		addItem( _lastShapeAsGraphicsItem );
//	}
//
//	else if( true == shapeButtons[Architecture::Shapes::eArrowDouble]->isChecked() )
//	{
//    	QString shapeName = Architecture::Shapes::shapeNames[ Architecture::Shapes::eText ];
//
//		_lastShape = Shapes::Factory().instanciate( shapeName );
//		_lastShape->setScenePosition( mouseEvent->scenePos() );
//		
//		addItem( _lastShapeAsGraphicsItem );
//	}
//
//	else if( true == shapeButtons[Architecture::Shapes::eInterface]->isChecked() )
//	{
//    	QString shapeName = Architecture::Shapes::shapeNames[ Architecture::Shapes::eText ];
//
//		_lastShape = Shapes::Factory().instanciate( shapeName );
//		_lastShape->setScenePosition( mouseEvent->scenePos() );
//		
//		addItem( _lastShapeAsGraphicsItem );
//	}
//
//	else if( true == shapeButtons[Architecture::Shapes::eAbstractClass]->isChecked() )
//	{
//    	QString shapeName = Architecture::Shapes::shapeNames[ Architecture::Shapes::eText ];
//
//		_lastShape = Shapes::Factory().instanciate( shapeName );
//		_lastShape->setScenePosition( mouseEvent->scenePos() );
//		
//		addItem( _lastShapeAsGraphicsItem );
//	}
//
//	else if( true == shapeButtons[Architecture::Shapes::eClass]->isChecked() )
//	{
//    	QString shapeName = Architecture::Shapes::shapeNames[ Architecture::Shapes::eText ];
//
//		_lastShape = Shapes::Factory().instanciate( shapeName );
//		_lastShape->setScenePosition( mouseEvent->scenePos() );
//		
//		addItem( _lastShapeAsGraphicsItem );
//	}

	update( sceneRect() );

	QGraphicsScene::mousePressEvent( mouseEvent );
}

void RendererGraphicsScene::mouseMoveEvent( QGraphicsSceneMouseEvent* mouseEvent )
{
/*
	if( (Qt::LeftButton == mouseEvent->buttons()) && (0 != _lastShapeAsGraphicsItem) )
	{
		qreal w = mouseEvent->scenePos().x() - mouseEvent->lastScenePos().x();
		qreal h = mouseEvent->scenePos().y() - mouseEvent->lastScenePos().y();

		// draws an arrow
		if
		(
		    true == shapeButtons[Architecture::Shapes::eArrow]->isChecked()
		    ||
		    true == shapeButtons[Architecture::Shapes::eArrowDouble]->isChecked()
		)
    		_lastShape->resize( w, h );

		update( sceneRect() );
	}
*/
	QGraphicsScene::mouseMoveEvent( mouseEvent );
}

void RendererGraphicsScene::mouseReleaseEvent( QGraphicsSceneMouseEvent* mouseEvent )
{
/*
	if( (0 != _lastShapeAsGraphicsItem) && (true == shapeButtons[Shapes::eLine]->isChecked()) )
	{
		QPointF startPoint = _startPoint;
		QPointF endPoint   = mouseEvent->scenePos();
		QPointF scenePos   = _lastShapeAsGraphicsItem->pos();

		removeItem( _lastShapeAsGraphicsItem );
		delete _lastShapeAsGraphicsItem;

		Line*	line = new Line();

		if( (startPoint.x() < endPoint.x()) && (startPoint.y() < endPoint.y()) )
		{
				line->setLineType( Line::eTopLeftBottomRight );
				line->setGeometry( QRectF(startPoint,endPoint) );
		}
		else
		if( (startPoint.x() < endPoint.x()) && (startPoint.y() > endPoint.y()) )
		{
				line->setLineType( Line::eBottomLeftTopRight );
				line->setGeometry( QRectF(
									QPointF(startPoint.x(),endPoint  .y()),
									QPointF(endPoint  .x(),startPoint.y())
								) );
		}
		else
		if( (startPoint.x() > endPoint.x()) && (startPoint.y() > endPoint.y()) )
		{
				line->setLineType( Line::eBottomRightTopLeft );
				line->setGeometry( QRectF(
									QPointF(endPoint  .x(),endPoint  .y()),
									QPointF(startPoint.x(),startPoint.y())
								) );
		}
		else
		if( (startPoint.x() > endPoint.x()) && (startPoint.y() < endPoint.y()) )
		{
				line->setLineType( Line::eTopRightBottomLeft );
				line->setGeometry( QRectF(
									QPointF(endPoint  .x(),startPoint.y()),
									QPointF(startPoint.x(),endPoint  .y())
								) );
		}
				line->setScenePosition( scenePos );

		addItem( line );
	}

	else if( (0 != _lastShapeAsGraphicsItem) && (true == shapeButtons[Shapes::eArrow]->isChecked()) )
	{
		QPointF startPoint = _startPoint;
		QPointF endPoint   = mouseEvent->scenePos();
		QPointF scenePos   = _lastShapeAsGraphicsItem->pos();

		removeItem( _lastShapeAsGraphicsItem );
		delete _lastShapeAsGraphicsItem;

		Arrow*	arrow = new Arrow();

		if( (startPoint.x() < endPoint.x()) && (startPoint.y() < endPoint.y()) )
		{
				arrow->setLineType( Line::eTopLeftBottomRight );
				arrow->setGeometry( QRectF(startPoint,endPoint) );
		}
		else
		if( (startPoint.x() < endPoint.x()) && (startPoint.y() > endPoint.y()) )
		{
				arrow->setLineType( Line::eBottomLeftTopRight );
				arrow->setGeometry( QRectF(
									QPointF(startPoint.x(),endPoint  .y()),
									QPointF(endPoint  .x(),startPoint.y())
								) );
		}
		else
		if( (startPoint.x() > endPoint.x()) && (startPoint.y() > endPoint.y()) )
		{
				arrow->setLineType( Line::eBottomRightTopLeft );
				arrow->setGeometry( QRectF(
									QPointF(endPoint  .x(),endPoint  .y()),
									QPointF(startPoint.x(),startPoint.y())
								) );
		}
		else
		if( (startPoint.x() > endPoint.x()) && (startPoint.y() < endPoint.y()) )
		{
				arrow->setLineType( Line::eTopRightBottomLeft );
				arrow->setGeometry( QRectF(
									QPointF(endPoint  .x(),startPoint.y()),
									QPointF(startPoint.x(),endPoint  .y())
								) );
		}
				arrow->setScenePosition( scenePos );

		addItem( arrow );
	}
*/
	QGraphicsScene::mouseReleaseEvent( mouseEvent );
}

void RendererGraphicsScene::drawBackground( QPainter* painter, const QRectF& rect )
{
	QGraphicsScene::drawBackground( painter, rect );

	painter->setBrush( Qt::black );
	painter->drawRect( _size._x+2, _size._y+2, _size._w, _size._h );

	painter->setBrush( Qt::white );
	painter->drawRect( _size._x, _size._y, _size._w, _size._h );
}
