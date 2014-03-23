
// qt headers
#include <QtGui/QPainter>
#include <QtGui/QStyleOption>

// local headers
#include "enemytarget.h"

EnemyTarget::EnemyTarget():
	GameObject()
{
	_objectType = ObjectEnemyTarget;
}

EnemyTarget::~EnemyTarget()
{}

void EnemyTarget::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	painter->setPen( Qt::NoPen );
	painter->setBrush( isSelected() ? Qt::magenta : Qt::darkMagenta );
    painter->drawEllipse( -_width/2, -_height/2, _width, _height );
	painter->setPen( QColor(qRgb(0,0,0)) );
	painter->drawText( -3, 4, "T" );
}

QPainterPath EnemyTarget::shape() const
{
	QPainterPath path;
	path.addEllipse(boundingRect());
	
	return path;
}
