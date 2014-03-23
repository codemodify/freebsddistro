
// local headers
#include "panel.h"

Panel::Panel( QWidget* parent ):
	QWidget(parent)
{
	_horizontaLayout = new QHBoxLayout( this );
}

Panel::~Panel()
{
}

