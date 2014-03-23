
// Qt headers
#include <QtGui/QMouseEvent>

// local headers
#include "enhancedqwidget.h"

EnhancedQWidget::EnhancedQWidget( QWidget* parent ):
	QWidget(parent)
{
}

EnhancedQWidget::~EnhancedQWidget()
{
}

void EnhancedQWidget::mousePressEvent( QMouseEvent* event )
{
	emit mouseButtonPress( event );

	_startDragPoint = event->pos();

	event->accept();
}

void EnhancedQWidget::mouseMoveEvent( QMouseEvent* event )
{
	emit dragged( event->globalPos() - _startDragPoint );

	event->accept();
}

void EnhancedQWidget::mouseReleaseEvent( QMouseEvent* event )
{
	emit mouseButtonRelease( event );
}

