#ifndef __Shapes_H__
#define __Shapes_H__

#include <QtGui/QToolButton>

namespace Shapes
{
	typedef enum ShapeType
	{
		eShapeBegin = 0,
	
		eMouse=0, eText, eLine, eArrow, eEllipse, eRectangle,

		eShapeCount,
		eUsupportedShape = eShapeCount
	};

	static char* shapeNames [ eShapeCount ] = { "mouse", "text", "line", "arrow", "ellipse", "rectangle" };
}

extern QToolButton*	shapeButtons[ Shapes::eShapeCount ];
extern QColor			shapeColor;

#endif
