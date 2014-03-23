
// qt headers
#include <QtGui/QPainter>
#include <QtGui/QStyleOption>

// local headers
#include "metarget.h"

MeTarget::MeTarget():
	GameObject()
{
	_objectType = ObjectTarget;
}

MeTarget::~MeTarget()
{}

void MeTarget::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	painter->setPen( Qt::NoPen );
	painter->setBrush( isSelected() ? Qt::cyan : Qt::darkCyan );
    painter->drawEllipse( -_width/2, -_height/2, _width, _height );
	painter->setPen( QColor(qRgb(0,0,0)) );
	painter->drawText( -3, 4, "T" );
}

QPainterPath MeTarget::shape() const
{
	QPainterPath path;
	path.addEllipse(boundingRect());
	
	return path;
}
