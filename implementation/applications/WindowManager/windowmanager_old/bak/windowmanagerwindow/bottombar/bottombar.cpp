
// local headers
#include "bottombar.h"

BottomBar::BottomBar():
	QWidget(0)
{
	setWindowFlags( Qt::FramelessWindowHint|Qt::Tool );
	setFixedHeight( BOTTOM_HEIGHT );

	_horizontaLayout = new QHBoxLayout(this);
	_leftCorner = new EnhancedQLabel();
	_rightCorner = new EnhancedQLabel();

	_horizontaLayout->addWidget( _leftCorner );
	_horizontaLayout->addStretch();
	_horizontaLayout->addWidget( _rightCorner );

	connect( _leftCorner, SIGNAL(labelDragged(int,int)), this, SLOT(resizeLeft(int,int)) );
	connect( _rightCorner, SIGNAL(labelDragged(int,int)), this, SLOT(resizeRight(int,int)) );
}

BottomBar::~BottomBar()
{
}

