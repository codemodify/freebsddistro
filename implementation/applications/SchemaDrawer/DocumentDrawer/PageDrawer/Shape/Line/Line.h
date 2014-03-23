#ifndef __Line_H__
#define __Line_H__

// local headers
#include "DocumentDrawer/PageDrawer/Shape/Shape.h"


class Line : public Shape
{
	public:
		Line();

		typedef enum{ eTopLeftBottomRight, eBottomRightTopLeft, eBottomLeftTopRight, eTopRightBottomLeft } LineType;
		void setLineType( LineType lineType );


	public:

		// paint
		virtual void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget );

		// serialization
		static Shape* fromString( QString& objectAsString );
		void	toString( QString& objectAsString );

		// duplication
		virtual Shape* duplicate();

	protected:
		LineType _lineType;

		QPointF _startPoint;
		QPointF _endPoint;

};

#endif
