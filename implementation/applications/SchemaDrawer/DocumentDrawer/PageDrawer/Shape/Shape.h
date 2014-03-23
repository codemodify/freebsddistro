#ifndef __Shape_H__
#define __Shape_H__

// Qt headers
#include <QtGui/QGraphicsRectItem>
#include <QtGui/QBrush>
#include <QtGui/QPainter>
#include <QtGui/QStyleOptionGraphicsItem>
#include <QtGui/QFontMetricsF>


class Shape : public QGraphicsRectItem
{
	public:
		Shape();
		virtual ~Shape();

	// paint
	public:
		virtual void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget );


	// serialization
	public:
		static	Shape*	fromString	( QString& objectAsString );
		virtual	void		toString		( QString& objectAsString );

	protected:
		static	void		fromString	( QString& objectAsString, Shape* intoShape );


	// duplication
	public:
		virtual Shape* duplicate() = 0;

	protected:
		void copyPropertiesIntoObject( Shape* intoShape );


	// shape's properties - setters
	public:
		virtual void setShapeName			( QString shapeName					);

		virtual void setText				( QString text						);
		virtual void setTextColor			( QColor textColor						);

		virtual void setLineColor			( QColor lineColor						);
		virtual void setFillColor			( QColor fillColor						);

		virtual void setScenePosition		( QPointF scenePosition				);
		virtual void setGeometry			( QRectF geometry						);
		//virtual void setTopLeftBottomRight	( QPointF topLeft, QPointF bottomRight	);
		virtual void resize				( qreal deltaW, qreal deltaH			);


	// shape's properties - getters
	public:
		virtual QString	getShapeName		();

		virtual QString	getText			();
		virtual QColor	getTextColor		();

		virtual QColor	getLineColor		();
		virtual QColor	getFillColor		();

		virtual QPointF	getScenePosition	();
		virtual QRectF	getGeometry		();


	// shape's properties
	protected:
		QString	_shapeName;

		QString	_text;
		QColor	_textColor;

		QColor	_lineColor;
		QColor	_fillColor;

};

#endif
