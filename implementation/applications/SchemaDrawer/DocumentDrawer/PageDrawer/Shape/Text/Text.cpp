
// Qt headers
#include <QBrush>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QFontMetricsF>

// local headers
#include "Text.h"


Text::Text() :
	Shape()
{
	setShapeName( "Text" );
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- paint
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void Text::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	Shape::paint( painter, option, widget );
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- serialization
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
Shape* Text::fromString( QString& objectAsString )
{
	if( false == objectAsString.startsWith("Text") )
		return 0;

	Text* text = new Text();

	Shape::fromString( objectAsString, text );

	return text;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- duplication
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
Shape* Text::duplicate()
{
	Text* text = new Text();

	copyPropertiesIntoObject( text );

	return text;
}
