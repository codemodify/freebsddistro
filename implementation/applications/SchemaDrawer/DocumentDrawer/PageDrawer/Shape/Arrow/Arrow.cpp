
// system
#include <math.h>

// local headers
#include "Arrow.h"


//Arrow::Arrow( QGraphicsItem* startItem, QGraphicsItem* endItem ) :
Arrow::Arrow() :
	Line()
{
	setShapeName( "Arrow" );
	setText( "" );
	setGeometry( QRectF(-5,-5,10,10) );

	myStartItem = 0;
	myEndItem   = 0;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- paint
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void Arrow::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	QPen	linePen( getLineColor() );
		linePen.setWidthF( 1.5 );

	QBrush fillBrush( getLineColor() );

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


	// draw the line
	painter->setPen( linePen );
	painter->drawLine( _startPoint, _endPoint );

	// draw the arrow head
	QLineF line( _endPoint, _startPoint );

	const qreal Pi = 3.14;
	const qreal c_arrowSize = 15;

	double angle = ::acos( line.dx() / line.length() );
	if( line.dy() >= 0 )
		angle = (Pi * 2) - angle;

    QPointF arrowP1 = line.p1() + QPointF( sin(angle + Pi      / 3) * c_arrowSize, cos(angle + Pi      / 3) * c_arrowSize );
    QPointF arrowP2 = line.p1() + QPointF( sin(angle + Pi - Pi / 3) * c_arrowSize, cos(angle + Pi - Pi / 3) * c_arrowSize );

    _arrowHead.clear();
    _arrowHead << line.p1() << arrowP1 << arrowP2;
    
    painter->setPen  ( linePen   );
    painter->setBrush( fillBrush );
    painter->drawPolygon( _arrowHead );

	// draw text
	Shape::paint( painter, option, widget );
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- serialization
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
Shape* Arrow::fromString( QString& objectAsString )
{
	if( false == objectAsString.startsWith("Arrow") )
		return 0;

	Arrow* arrow = new Arrow();

	// de-serialize the common part
	Shape::fromString( objectAsString, arrow );

	// extract the additions
	int lineType = objectAsString.split(";").value( 7 ).toInt();
	arrow->setLineType( static_cast<Line::LineType>(lineType) );

	return arrow;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- duplication
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
Shape* Arrow::duplicate()
{
	Arrow* arrow = new Arrow();

	copyPropertiesIntoObject( arrow );

	arrow->setLineType( _lineType );

	return arrow;
}





























/*
QRectF Arrow::boundingRect() const
{
    qreal extra = ( pen().width() + 20 ) / 2.0;

    return QRectF( line().p1(), QSizeF(line().p2().x() - line().p1().x(), line().p2().y() - line().p1().y()) )
			.normalized()
			.adjusted(-extra, -extra, extra, extra);
}

QPainterPath Arrow::shape() const
{
	QPainterPath	path = QGraphicsLineItem::shape();
				path.addPolygon( _arrowHead );

    return path;
}
*/
