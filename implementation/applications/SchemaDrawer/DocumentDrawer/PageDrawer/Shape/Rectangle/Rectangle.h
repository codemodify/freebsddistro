#ifndef __Rectangle_H__
#define __Rectangle_H__

// local headers
#include "DocumentDrawer/PageDrawer/Shape/Shape.h"


class Rectangle : public Shape
{
	public:
		Rectangle();

	public:

		// paint
		void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget );

		// serialization
		static Shape* fromString( QString& objectAsString );

		// duplication
		Shape* duplicate();

};

#endif
