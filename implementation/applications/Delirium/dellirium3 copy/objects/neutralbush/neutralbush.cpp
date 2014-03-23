
// qt headers
#include <QtGui/QPainter>
#include <QtGui/QStyleOption>

// local headers
#include "neutralbush.h"

NeutralBush::NeutralBush():
	GameObject()
{
	_objectType = ObjectBush;
}

NeutralBush::~NeutralBush()
{}

void NeutralBush::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	painter->setPen( Qt::NoPen );
	painter->setBrush( isSelected() ? Qt::yellow : Qt::darkYellow );
    painter->drawEllipse( -_width/2, -_height/2, _width, _height );
	painter->setPen( QColor(qRgb(0,0,0)) );
	painter->drawText( -15, 4, "Bush" );
}

QPainterPath NeutralBush::shape() const
{
	QPainterPath path;
	path.addEllipse(boundingRect());
	
	return path;
}
