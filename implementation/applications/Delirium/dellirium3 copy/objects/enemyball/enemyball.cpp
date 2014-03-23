
// qt headers
#include <QtGui/QPainter>
#include <QtGui/QStyleOption>

// local headers
#include "enemyball.h"

EnemyBall::EnemyBall():
	GameObject()
{
	_objectType = ObjectEnemyBall;
}

EnemyBall::~EnemyBall()
{}

void EnemyBall::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	painter->setPen( Qt::NoPen );
	painter->setBrush( isSelected() ? Qt::red : Qt::darkRed );
    painter->drawEllipse( -_width/2, -_height/2, _width, _height );
}

QPainterPath EnemyBall::shape() const
{
	QPainterPath path;
	path.addEllipse(boundingRect());
	
	return path;
}
