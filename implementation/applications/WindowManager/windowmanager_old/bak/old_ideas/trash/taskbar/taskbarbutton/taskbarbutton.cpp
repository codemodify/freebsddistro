
// local headers
#include "taskbarbutton.h"

TakBarButton::TakBarButton( QWidget* parent ):
	EnhancedQWidget(parent)
{
}

TakBarButton::~TakBarButton()
{
}

void TakBarButton::paintEvent( QPaintEvent* paintEvent )
{
	EnhancedQWidget::paintEvent( paintEvent );

	QPainter painter( this );
	painter.drawText( 10, 20, _text );
}

