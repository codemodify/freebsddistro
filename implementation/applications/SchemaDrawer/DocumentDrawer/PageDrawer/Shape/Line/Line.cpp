
// local headers
#include "Line.h"


Line::Line() :
	Shape(),
	_lineType( eTopLeftBottomRight )
{
	setShapeName( "Line" );
	setText( "" );
	setGeometry( QRectF(0,0,0,0) );
}

void Line::setLineType( LineType lineType )
{
	_lineType = lineType;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- paint
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void Line::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	QPen	linePen( getLineColor() );
		linePen.setWidthF( 1.5 );

	switch( _lineType )
	{
		case eTopLeftBottomRight:
			_startPoint = getGeometry().topLeft();
			_endPoint   = getGeometry().bottomRight();
			break;

		case eBottomRightTopLeft:
			_startPoint = getGeometry().bottomRight();
			_endPoint   = getGeometry().topLeft();
			break;

		case eBottomLeftTopRight:
			_startPoint = getGeometry().bottomLeft();
			_endPoint   = getGeometry().topRight();
			break;

		case eTopRightBottomLeft:
			_startPoint = getGeometry().topRight();
			_endPoint   = getGeometry().bottomLeft();
			break;

		default:
			_startPoint = getGeometry().topLeft();
			_endPoint   = getGeometry().bottomRight();
			break;			
	}

	painter->setPen( linePen );
	painter->drawLine( _startPoint, _endPoint );

	Shape::paint( painter, option, widget );
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- serialization
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
Shape* Line::fromString( QString& objectAsString )
{
	if( false == objectAsString.startsWith("Line") )
		return 0;

	Line* line = new Line();

	// de-serialize the common part
	Shape::fromString( objectAsString, line );

	// extract the additions
	int lineType = objectAsString.split(";").value( 7 ).toInt();
	line->setLineType( static_cast<Line::LineType>(lineType) );

	return line;
}

void	Line::toString( QString& objectAsString )
{
	// serialize the common part
	Shape::toString( objectAsString );

	// add new additions
	objectAsString = QString("%1;%2").arg( objectAsString ).arg( _lineType );
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- duplication
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
Shape* Line::duplicate()
{
	Line* line = new Line();

	copyPropertiesIntoObject( line );

	line->setLineType( _lineType );

	return line;
}
