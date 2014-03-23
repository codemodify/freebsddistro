
// qt headers
#include <QtGui/QPainter>
#include <QtGui/QStyleOption>

// local headers
#include "neutralwall.h"

NeutralWall::NeutralWall():
	GameObject()
{
	_objectType = ObjectWall;
	
	_width = _height = 20;
}

NeutralWall::~NeutralWall()
{}

void NeutralWall::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	painter->setPen( Qt::NoPen );
	painter->setBrush( isSelected() ? Qt::yellow : Qt::darkYellow );
    painter->drawRect( -_width/2, -_height/2, _width, _height );
}

QPainterPath NeutralWall::shape() const
{
	QPainterPath path;
	path.addRect(boundingRect());
	
	return path;
}
