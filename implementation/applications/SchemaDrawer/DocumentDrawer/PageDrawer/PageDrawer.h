#ifndef __PageDrawer_H__
#define __PageDrawer_H__

// Qt headers
#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsItem>

// local headers
#include "PageFormat.h"

// forwards
class Properties;

class PageDrawer : public QGraphicsScene
{
	Q_OBJECT

	public:
		PageDrawer();
		~PageDrawer();

	public:
		void              setPageFormat( PageFormat::Format format );
		PageFormat::Size& getPageFormat();

		void setObjectsFromStringList( QStringList& objectsAsStringList );
		void getObjectsAsStringList  ( QStringList& objectsAsStringList );

	// overrides
	protected:
		void mousePressEvent	( QGraphicsSceneMouseEvent* mouseEvent );
		void mouseMoveEvent	( QGraphicsSceneMouseEvent* mouseEvent );
		void mouseReleaseEvent	( QGraphicsSceneMouseEvent* mouseEvent );

		void drawBackground	( QPainter* painter, const QRectF& rect );

	private:
		QPointF			_startPoint;
		QGraphicsItem*	_lastGraphicsItem;
		Properties*		_properties;

		PageFormat::Size&	_size;
};

#endif
