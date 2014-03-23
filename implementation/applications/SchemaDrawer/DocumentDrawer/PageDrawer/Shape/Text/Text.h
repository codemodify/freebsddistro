#ifndef __Text_H__
#define __Text_H__

// local headers
#include "DocumentDrawer/PageDrawer/Shape/Shape.h"

class Text : public Shape
{
	public:
		Text();

	public:

		// paint
		void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget );

		// serialization
		static Shape* fromString( QString& objectAsString );

		// duplication
		Shape* duplicate();

};

#endif
