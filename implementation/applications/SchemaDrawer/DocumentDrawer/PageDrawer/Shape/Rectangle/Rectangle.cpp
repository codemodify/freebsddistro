
// Qt headers
#include <QBrush>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QFontMetricsF>

// local headers
#include "Rectangle.h"


Rectangle::Rectangle() :
	Shape()
{
	setShapeName( "Rectangle" );
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- paint
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void Rectangle::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	QGraphicsRectItem::paint( painter, option, widget );

	Shape::paint( painter, option, widget );
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- serialization
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
Shape* Rectangle::fromString( QString& objectAsString )
{
	if( false == objectAsString.startsWith("Rectangle") )
		return 0;

	Rectangle* rectangle = new Rectangle();

	Shape::fromString( objectAsString, rectangle );

	return rectangle;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- duplication
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
Shape* Rectangle::duplicate()
{
	Rectangle* rectangle = new Rectangle();

	copyPropertiesIntoObject( rectangle );

	return rectangle;
}
