#ifndef __Ellipse_H__
#define __Ellipse_H__

// local headers
#include "DocumentDrawer/PageDrawer/Shape/Shape.h"


class Ellipse : public Shape
{
	public:
		Ellipse();

	public:

		// paint
		void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget );

		// serialization
		static Shape* fromString( QString& objectAsString );

		// duplication
		Shape* duplicate();

};

#endif
