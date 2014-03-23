
// qt headers
#include <QtGui/QCursor>

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
