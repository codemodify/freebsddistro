
// Qt headers
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QPainter>

// local headers
#include "PageDrawer.h"

#include "Shapes.h"

#include "Properties/Properties.h"

#include "Shape/Arrow/Arrow.h"
#include "Shape/Ellipse/Ellipse.h"
#include "Shape/Line/Line.h"
#include "Shape/Rectangle/Rectangle.h"
#include "Shape/Text/Text.h"


QToolButton*      shapeButtons[ Shapes::eShapeCount ];
PageFormat::Size* pageFormats [ PageFormat::eCount  ] = { new PageFormat::A4() };
QColor			shapeColor;


PageDrawer::PageDrawer():
	QGraphicsScene( 0 ),
	_properties( 0 ),
	_size( *( pageFormats[PageFormat::eA4] ) )
{
	setPageFormat( PageFormat::eA4 );

	setBackgroundBrush( Qt::gray );

	_properties = new Properties();
	addItem( _properties );
}

PageDrawer::~PageDrawer()
{}

void PageDrawer::setPageFormat( PageFormat::Format format )
{
	_size = *( pageFormats[format] );

	setSceneRect( _size._x, _size._y-60, _size._w, _size._h+120 );
}

PageFormat::Size& PageDrawer::getPageFormat()
{
	return _size;
}

void PageDrawer::setObjectsFromStringList( QStringList& objectsAsStringList )
{
	foreach( QString object, objectsAsStringList )
	{
		Shape* shape = 0;

		if( shape=Text     ::fromString(object), 0 == shape )
		if( shape=Line     ::fromString(object), 0 == shape )
		if( shape=Arrow    ::fromString(object), 0 == shape )
		if( shape=Ellipse  ::fromString(object), 0 == shape )
		if( shape=Rectangle::fromString(object), 0 == shape )
			continue;		
		
		addItem( shape );
	}
}

void PageDrawer::getObjectsAsStringList( QStringList& objectsAsStringList )
{
	QList< QGraphicsItem* > graphicsItems = items();

	foreach( QGraphicsItem* graphicsItem, graphicsItems )
	{
		Shape* shape = dynamic_cast<Shape*>( graphicsItem );
		if( shape )
		{
			QString objectAsString;

			shape->toString( objectAsString );

			objectsAsStringList.append( objectAsString );
		}
	}
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- overrides
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void PageDrawer::mousePressEvent( QGraphicsSceneMouseEvent* mouseEvent )
{
	_startPoint = mouseEvent->scenePos();

	// mouse mode, select items, do some operations on them
	if( true == shapeButtons[Shapes::eMouse]->isChecked() )
	{
		_lastGraphicsItem = itemAt( mouseEvent->scenePos() );
		if( _lastGraphicsItem != _properties )
		{
			_properties->setTarget( dynamic_cast<Shape*>(_lastGraphicsItem) );
			_properties->setZValue( items().count() );
		}
	}

	// shapes mode, add some shapes on the schene
	else if( true == shapeButtons[Shapes::eText]->isChecked() )
	{
		_lastGraphicsItem = new Text();
		_lastGraphicsItem->setPos( mouseEvent->scenePos() );
		addItem( _lastGraphicsItem );
	}

	else if( true == shapeButtons[Shapes::eLine]->isChecked() )
	{
		_lastGraphicsItem = new Line();
		_lastGraphicsItem->setPos( mouseEvent->scenePos() );
		addItem( _lastGraphicsItem );
	}

	else if( true == shapeButtons[Shapes::eArrow]->isChecked() )
	{
		_lastGraphicsItem = new Arrow();
		_lastGraphicsItem->setPos( mouseEvent->scenePos() );
		addItem( _lastGraphicsItem );
	}

	else if( true == shapeButtons[Shapes::eEllipse]->isChecked() )
	{
		_lastGraphicsItem = new Ellipse();
		_lastGraphicsItem->setPos( mouseEvent->scenePos() );
		addItem( _lastGraphicsItem );
	}

	else if( true == shapeButtons[Shapes::eRectangle]->isChecked() )
	{
		_lastGraphicsItem = new Rectangle();
		_lastGraphicsItem->setPos( mouseEvent->scenePos() );
		addItem( _lastGraphicsItem );
	}

	update( sceneRect() );

	QGraphicsScene::mousePressEvent( mouseEvent );
}

void PageDrawer::mouseMoveEvent( QGraphicsSceneMouseEvent* mouseEvent )
{
	if( (Qt::LeftButton == mouseEvent->buttons()) && (0 != _lastGraphicsItem) )
	{
		qreal w = mouseEvent->scenePos().x() - mouseEvent->lastScenePos().x();
		qreal h = mouseEvent->scenePos().y() - mouseEvent->lastScenePos().y();

		// draws a line 
		if( true == shapeButtons[Shapes::eLine]->isChecked() 	)
		{
			Shape*	shape = dynamic_cast<Shape*>( _lastGraphicsItem );
					shape->resize( w, h );
		}

		// draws an arrow
		else if( true == shapeButtons[Shapes::eArrow]->isChecked() 	)
		{
			Shape*	shape = dynamic_cast<Shape*>( _lastGraphicsItem );
					shape->resize( w, h );
		}

		update( sceneRect() );
	}

	QGraphicsScene::mouseMoveEvent( mouseEvent );
}

void PageDrawer::mouseReleaseEvent( QGraphicsSceneMouseEvent* mouseEvent )
{
	if( (0 != _lastGraphicsItem) && (true == shapeButtons[Shapes::eLine]->isChecked()) )
	{
		QPointF startPoint = _startPoint;
		QPointF endPoint   = mouseEvent->scenePos();
		QPointF scenePos   = _lastGraphicsItem->pos();

		removeItem( _lastGraphicsItem );
		delete _lastGraphicsItem;

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

	else if( (0 != _lastGraphicsItem) && (true == shapeButtons[Shapes::eArrow]->isChecked()) )
	{
		QPointF startPoint = _startPoint;
		QPointF endPoint   = mouseEvent->scenePos();
		QPointF scenePos   = _lastGraphicsItem->pos();

		removeItem( _lastGraphicsItem );
		delete _lastGraphicsItem;

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

	QGraphicsScene::mouseReleaseEvent( mouseEvent );
}

void PageDrawer::drawBackground( QPainter* painter, const QRectF& rect )
{
	QGraphicsScene::drawBackground( painter, rect );

	painter->setBrush( Qt::black );
	painter->drawRect( _size._x+2, _size._y+2, _size._w, _size._h );

	painter->setBrush( Qt::white );
	painter->drawRect( _size._x, _size._y, _size._w, _size._h );
}

