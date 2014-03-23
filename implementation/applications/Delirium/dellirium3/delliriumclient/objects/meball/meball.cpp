
// qt headers
#include <QtGui/QPainter>
#include <QtGui/QStyleOption>

// local headers
#include "meball.h"

MeBall::MeBall():
	GameObject()
{
	_objectType = ObjectBall;
}

MeBall::~MeBall()
{}

void MeBall::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	painter->setPen( Qt::NoPen );
	painter->setBrush( isSelected() ? Qt::green : Qt::darkGreen );
    painter->drawEllipse( -_width/2, -_height/2, _width, _height );
}

QPainterPath MeBall::shape() const
{
	QPainterPath path;
	path.addEllipse(boundingRect());
	
	return path;
}
