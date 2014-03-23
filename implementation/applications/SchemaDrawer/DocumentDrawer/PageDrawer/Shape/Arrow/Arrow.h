#ifndef __Arrow_H__
#define __Arrow_H__

// Qt headers
#include <QtGui/QGraphicsLineItem>

// local headers
#include "DocumentDrawer/PageDrawer/Shape/Line/Line.h"


class Arrow : public Line
{
	public:
		Arrow();
		//Arrow( QGraphicsItem* startItem, QGraphicsItem* endItem );

	public:

		// paint
		void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget );

		// serialization
		static Shape* fromString( QString& objectAsString );

		// duplication
		Shape* duplicate();

	private:
		QGraphicsItem*	myStartItem;
		QGraphicsItem*	myEndItem;
		QPolygonF 		_arrowHead;

};

#endif
