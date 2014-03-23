
// local headers
#include "Shape.h"

Shape::Shape() :
	QGraphicsRectItem()
{
	setShapeName( "Shape" );

	setText( "Some Text" );
	setTextColor( QColor::fromHsv(rand()%256,255,190) );

	setText( "Some Text" );
	setTextColor( QColor::fromHsv(rand()%256,255,190) );

	setLineColor( QColor::fromHsv(rand()%256,255,190) );
	setFillColor( QColor::fromHsv(rand()%256,255,190) );

	setScenePosition( QPointF(100,100) );
	setGeometry( QRectF(-100,-50,200,100) );
}

Shape::~Shape()
{}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- paint
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void Shape::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	QPen textPen( getTextColor() );
	
	painter->setPen( textPen );
	painter->drawText
	(
		option->rect.x() + option->rect.width ()/2 - QFontMetricsF(painter->font()).width ( getText() )/2, 
		option->rect.y() + option->rect.height()/2 + QFontMetricsF(painter->font()).height(           )/3,
		getText()
	);
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- serialization
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void Shape::toString( QString& objectAsString )
{
	objectAsString = QString( "%1;%2;%3;%4;%5;%6;%7" );

	objectAsString = objectAsString.arg( getShapeName() );

	objectAsString = objectAsString.arg( getText() );
	objectAsString = objectAsString.arg( QString("%1,%2,%3").arg( getTextColor().red() ).arg( getTextColor().green() ).arg( getTextColor().blue() ) );

	objectAsString = objectAsString.arg( QString("%1,%2,%3").arg( getLineColor().red() ).arg( getLineColor().green() ).arg( getLineColor().blue() ) );
	objectAsString = objectAsString.arg( QString("%1,%2,%3").arg( getFillColor().red() ).arg( getFillColor().green() ).arg( getFillColor().blue() ) );

	objectAsString = objectAsString.arg
	(
		QString("%1,%2")		.arg( getScenePosition().x() )
							.arg( getScenePosition().y() )
	);

	objectAsString = objectAsString.arg
	(
		QString("%1,%2,%3,%4")	.arg( getGeometry().x()		)
							.arg( getGeometry().y() 		)
							.arg( getGeometry().width()		)
							.arg( getGeometry().height()	)
	);
}

void Shape::fromString( QString& objectAsString, Shape* intoShape )
{
	// extract properties
	QStringList properties = objectAsString.split( ";" );

	QString objectType			= properties.value( 0 );

	QString objectText			= properties.value( 1 );
	QString objectTextColor	= properties.value( 2 );

	QString objectLineColor	= properties.value( 3 );
	QString objectFillColor	= properties.value( 4 );

	QString objectScenePosition	= properties.value( 5 );
	QString objectGeometry		= properties.value( 6 );


	intoShape->setText			( objectText );

	intoShape->setTextColor	( QColor(objectTextColor.split(",").value(0).toInt(),
									objectTextColor.split(",").value(1).toInt(),
									objectTextColor.split(",").value(2).toInt()) );

	intoShape->setLineColor	( QColor(objectLineColor.split(",").value(0).toInt(),
									objectLineColor.split(",").value(1).toInt(),
									objectLineColor.split(",").value(2).toInt()) );

	intoShape->setFillColor	( QColor(objectFillColor.split(",").value(0).toInt(),
									objectFillColor.split(",").value(1).toInt(),
									objectFillColor.split(",").value(2).toInt()) );

	intoShape->setScenePosition	( QPoint(objectScenePosition.split(",").value(0).toDouble(),
									objectScenePosition.split(",").value(1).toDouble()	) );

	intoShape->setGeometry		( QRect (objectGeometry.split(",").value(0).toDouble(),
									objectGeometry.split(",").value(1).toDouble(),
									objectGeometry.split(",").value(2).toDouble(),
									objectGeometry.split(",").value(3).toDouble()) );
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- duplication
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void Shape::copyPropertiesIntoObject( Shape* intoShape )
{
	intoShape->setText			( getText()			);
	intoShape->setTextColor	( getTextColor()		);

	intoShape->setLineColor	( getLineColor()		);
	intoShape->setFillColor	( getFillColor()		);

	intoShape->setScenePosition	( getScenePosition()	);
	intoShape->setGeometry		( getGeometry()		);

}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- shape's properties - setters
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void Shape::setShapeName	( QString shapeName	){ _shapeName		= shapeName;		}

void Shape::setText		( QString text		){ _text			= text;			}
void Shape::setTextColor	( QColor textColor		){ _textColor		= textColor;		}

void Shape::setLineColor( QColor lineColor )
{
	_lineColor = lineColor;

	setPen( QPen(lineColor) );
}

void Shape::setFillColor( QColor fillColor )
{
	_fillColor = fillColor;

	setBrush( QBrush(fillColor) );
}

void Shape::setScenePosition( QPointF scenePosition )
{
	setPos( scenePosition );
}

void Shape::setGeometry( QRectF geometry )
{
	setRect( -1*geometry.width()/2, -1*geometry.height()/2, geometry.width(), geometry.height() );
}

//void Shape::setTopLeftBottomRight( QPointF topLeft, QPointF bottomRight )
//{
//	setRect( topLeft.x(), topLeft.y(), bottomRight.x(), bottomRight.y() );
//
//	setPos( (bottomRight.x()-topLeft.x()) / 2, (bottomRight.y()-topLeft.y()) / 2 );
//}

void Shape::resize	( qreal deltaW, qreal deltaH )
{
	QRectF	newRect = rect();
			newRect.setWidth ( newRect.width()  + deltaW );
			newRect.setHeight( newRect.height() + deltaH );

	setGeometry( newRect );

	moveBy( deltaW/2, deltaH/2 );
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- shape's properties - getters
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
QString	Shape::getShapeName		(){ return _shapeName;		}

QString	Shape::getText			(){ return _text;			}
QColor	Shape::getTextColor		(){ return _textColor;		}

QColor	Shape::getLineColor		(){ return _lineColor;		}
QColor	Shape::getFillColor		(){ return _fillColor;		}

QPointF	Shape::getScenePosition	(){ return pos();			}
QRectF	Shape::getGeometry			(){ return rect();			}
