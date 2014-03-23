
// qt headers
#include <QtGui/QCursor>
#include <QtGui/QGraphicsSceneMouseEvent>

// local headers
#include "gameobject.h"

GameObject::GameObject():
	QGraphicsItem()
{
	_objectType = ObjectNone;

	setFlag( QGraphicsItem::ItemIsSelectable, true );
	setFlag( QGraphicsItem::ItemIsMovable, true );
	setCursor( QCursor(Qt::ArrowCursor) );
	
	_width = 40;
	_height = 40;
}

GameObject::~GameObject()
{}

QRectF GameObject::boundingRect() const
{
	return QRectF( -(_width/2), -(_height/2), _width, _height );
}

void GameObject::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
	QGraphicsItem::mousePressEvent( event );
}

void GameObject::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
	QGraphicsItem::mouseReleaseEvent( event );
	
	// here we make sure that the objects that will move are objects that are designed to move
	if( (ObjectBall!=_objectType) && (ObjectEnemyBall!=_objectType) )
		return;
	
	QLineF line( pos(), event->pos() );
	_pathToTraverse = line;
	
	_timerID = startTimer(200);
}

void GameObject::timerEvent( QTimerEvent* event )
{
	QPointF startPosition = pos();
	QPointF   endPosition = _pathToTraverse.p2();

	qreal x1 = startPosition.x();
	qreal y1 = startPosition.y();

	qreal x2 = endPosition.x();
	qreal y2 = endPosition.y();

	qreal shift = 1.0;
	qreal x;
	if( (x1<0) && (x2<0) )
		x = x1 < x2 ? x1 - shift : x1 + shift;
	else
		x = x1 < x2 ? x1 + shift : x1 - shift;
	
	qreal y = ((y2-y1)/(x2-x1))*(x-x1) + y1;

	if( startPosition != endPosition )
	{
		setPos( x, y );
	}
	else
	{
		killTimer( _timerID );
	}
}
