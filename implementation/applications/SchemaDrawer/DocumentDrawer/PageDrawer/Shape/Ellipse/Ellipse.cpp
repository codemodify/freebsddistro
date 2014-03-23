
// local headers
#include "Ellipse.h"


Ellipse::Ellipse() :
	Shape()
{
	setShapeName( "Ellipse" );
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- paint
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void Ellipse::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	painter->setPen  ( QPen(getLineColor())   );
	painter->setBrush( QBrush(getFillColor()) );

	painter->drawEllipse( getGeometry() );

	Shape::paint( painter, option, widget );
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- serialization
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
Shape* Ellipse::fromString( QString& objectAsString )
{
	if( false == objectAsString.startsWith("Ellipse") )
		return 0;

	Ellipse* ellipse = new Ellipse();

	Shape::fromString( objectAsString, ellipse );

	return ellipse;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- duplication
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
Shape* Ellipse::duplicate()
{
	Ellipse* ellipse = new Ellipse();

	copyPropertiesIntoObject( ellipse );

	return ellipse;
}
