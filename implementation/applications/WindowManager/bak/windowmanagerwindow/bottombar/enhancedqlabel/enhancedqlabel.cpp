
// local headers
#include "enhancedqlabel.h"

EnhancedQLabel::EnhancedQLabel( QWidget* parent ):
	QLabel(parent),
	_draggingMode(false)
{
}

EnhancedQLabel::~EnhancedQLabel()
{
}

void EnhancedQLabel::mousePressEvent( QMouseEvent* event )
{
	event->accept();

	_draggingMode = true;
}

void EnhancedQLabel::mouseMoveEvent( QMouseEvent* event )
{
	event->accept();

	if( true == _draggingMode )
		emit labelDragged( event->globalX(), event->globalY() );
}

void EnhancedQLabel::mouseReleaseEvent( QMouseEvent* event )
{
	event->accept();

	_draggingMode = false;
}

