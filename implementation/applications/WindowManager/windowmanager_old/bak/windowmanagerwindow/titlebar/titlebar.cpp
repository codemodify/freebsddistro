
// local headers
#include "titlebar.h"

TitleBar::TitleBar():
	QWidget(0)
{
	setWindowFlags( Qt::FramelessWindowHint|Qt::Tool );
	setFixedHeight( TITLE_HEIGHT );
	setMouseTracking( true );

	_horizontalLayout = new QHBoxLayout(this);
	_icon = new QLabel();
	_text = new QLabel();
	_minimizeButton = new QToolButton();
	_maximizeButton = new QToolButton();
	_closeButton = new QToolButton();

	_minimizeButton->setText( "_" );
	_maximizeButton->setText( "o" );
	_closeButton->setText( "x" );

	_horizontalLayout->addWidget( _icon );
	_horizontalLayout->addWidget( _text );
	_horizontalLayout->addStretch();
	_horizontalLayout->addWidget( _minimizeButton );
	_horizontalLayout->addWidget( _maximizeButton );
	_horizontalLayout->addWidget( _closeButton );

	connect( _minimizeButton, SIGNAL(clicked()), this, SLOT(minimizeButton()) );
	connect( _maximizeButton, SIGNAL(clicked()), this, SLOT(maximizeButton()) );
	connect( _closeButton, SIGNAL(clicked()), this, SLOT(closeButton()) );
}

TitleBar::~TitleBar()
{
}

QString TitleBar::getTitlebarText()
{
	return _text->text();
}

void TitleBar::setTitlebarText( QString text )
{
	_text->setText( text );
}

void TitleBar::mousePressEvent( QMouseEvent* event )
{
	_dragPosition = event->globalPos() - frameGeometry().topLeft();

	event->accept();
}

void TitleBar::mouseMoveEvent( QMouseEvent* event )
{
	if( event->buttons() )
	{
		QPoint point = event->globalPos() - _dragPosition;
		emit titleDragged( point.x(), point.y() );
		move( point );
	}

	event->accept();
}

