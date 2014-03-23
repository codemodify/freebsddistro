
// Qt headers
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QToolButton>
#include <QtCore/QEvent>

// local headers
#include "xwindow.h"
#include "helpers/xlibwrapper/xlibwrapper.h"

#define TITLE_SIZE 20
#define BOTTOM_BORDER_SIZE 3
#define BOTTOM_RESIZE_FRAME_WIDTH 20


#define XWINDOW "XWindow"

static XWindow* onTopWindow = 0;

XWindow::XWindow( Window window ) :
	EnhancedQWidget(),
	_window(window),
	_isShaded(false),
	_isMinimized(false),
	_isMaximized(false)
{

	// the top part
	_shade = new QToolButton();
	_shade->setArrowType( Qt::DownArrow );
	_shade->setAutoRaise( true );
	connect( _shade, SIGNAL(released()), this, SLOT(shadeWindow()) );

	_icon = new QLabel(); 

	_text = new QLabel();

	_minimize = new QToolButton();
	_minimize->setText( "min" );
	_minimize->setAutoRaise( true );
	connect( _minimize, SIGNAL(released()), this, SLOT(minimizeWindow()) );

	_maximize = new QToolButton();
	_maximize->setText( "max" );
	_maximize->setAutoRaise( true );
	connect( _maximize, SIGNAL(released()), this, SLOT(maximizeWindow()) );

	_close = new QToolButton();
	_close->setText( "X" );
	_close->setAutoRaise( true );
	connect( _close, SIGNAL(released()), this, SLOT(closeWindow()) );

	QHBoxLayout* topHorizontalLayout = new QHBoxLayout();
	topHorizontalLayout->setMargin( 0 );
	topHorizontalLayout->addWidget( _shade );
	topHorizontalLayout->addWidget( _icon );
	topHorizontalLayout->addStretch();
	topHorizontalLayout->addWidget( _text );
	topHorizontalLayout->addStretch();
	topHorizontalLayout->addWidget( _minimize );
	topHorizontalLayout->addWidget( _maximize );
	topHorizontalLayout->addWidget( _close );

	// the middle part
	XSetWindowBorderWidth( XLibWrapper::getDisplay(), _window, 0 );
	XSetWindowBorderWidth( XLibWrapper::getDisplay(), winId(), 0 );
	XReparentWindow( XLibWrapper::getDisplay(), _window, winId(), _x, _y+TITLE_SIZE );
	XSelectInput( XLibWrapper::getDisplay(), _window, EventMaskForNormalWindow );
	XAddToSaveSet( XLibWrapper::getDisplay(), _window );

	// the bottom part
	_leftResizeFrame = new EnhancedQWidget();
	_leftResizeFrame->setFixedSize( BOTTOM_RESIZE_FRAME_WIDTH, BOTTOM_BORDER_SIZE );
	connect( _leftResizeFrame, SIGNAL(dragged(QPoint)), this, SLOT(leftResize(QPoint)) );

	_rightResizeFrame = new EnhancedQWidget();
	_rightResizeFrame->setFixedSize( BOTTOM_RESIZE_FRAME_WIDTH, BOTTOM_BORDER_SIZE );
	connect( _rightResizeFrame, SIGNAL(dragged(QPoint)), this, SLOT(rightResize(QPoint)) );

	QHBoxLayout* bottomHorizontalLayout = new QHBoxLayout();
	bottomHorizontalLayout->setMargin( 0 );
	bottomHorizontalLayout->addWidget( _leftResizeFrame );
	bottomHorizontalLayout->addStretch();
	bottomHorizontalLayout->addWidget( _rightResizeFrame );

	// here we assemble all the parts above
	QVBoxLayout* verticalBoxLayout = new QVBoxLayout(this);
	verticalBoxLayout->setMargin( 0 );
	verticalBoxLayout->addLayout( topHorizontalLayout );
	verticalBoxLayout->addStretch( _height );
	verticalBoxLayout->addLayout( bottomHorizontalLayout );
	setLayout( verticalBoxLayout );

	connect( this, SIGNAL(dragged(QPoint)), this, SLOT(draggedToNewPosition(QPoint)) );
	connect( this, SIGNAL(mouseButtonPress(QMouseEvent*)), this, SLOT(raise()) );

	// apply different properties related to window's geometry
	applyProperties();
}

XWindow::~XWindow()
{
}

bool XWindow::isOnTop()
{
	return (this == onTopWindow);
}
bool XWindow::isMinimized()
{
	return _isMinimized;
}

QString XWindow::getText()
{
	std::string text;
	XLibWrapper::getWindowTitleText( _window, text );

	return QString( text.c_str() );
}

void XWindow::applyProperties()
{
	// getting the window's geometry that it wants to start with
	XLibWrapper::getWindowGeometry( _window, _x, _y, _width, _height );

	setGeometry( _x, _y, _width, _height+TITLE_SIZE+BOTTOM_BORDER_SIZE );

	XLibWrapper::moveWindow( _window, 0, 0+TITLE_SIZE );
	XLibWrapper::resizeWindow( _window, _width, _height-BOTTOM_BORDER_SIZE );

	_text->setText( getText() );
}

void XWindow::configureWindow( XConfigureRequestEvent* event )
{
	XLibWrapper::configureWindow( event );

	// here we have to adjust to the changes applies to window
	applyProperties();
}

void XWindow::propertyChanged( XPropertyEvent* event )
{
}

void XWindow::focus()
{
	XLibWrapper::focusWindow( _window );
}

void XWindow::show()
{
	QWidget::show();
	XLibWrapper::showWindow( _window );
}

void XWindow::draggedToNewPosition( QPoint newPosition )
{
	move( newPosition );

	_x = pos().x();
	_y = pos().y();

}

void XWindow::leftResize( QPoint newPosition )
{
	///int x1 = _x + _width;

	//_x = (newPosition).x();
	//_width = width + (x1 - x); 
	//_height = mapFromGlobal(newPosition).y() - TITLE_SIZE;

	//applyProperties();
}

void XWindow::rightResize( QPoint newPosition )
{
	_width = mapFromGlobal(newPosition).x() + _rightResizeFrame->width();
	_height = mapFromGlobal(newPosition).y() - TITLE_SIZE;

	applyProperties();
}

void XWindow::enterEvent( QEvent* event )
{
	XLibWrapper::focusWindow( _window );

	event->accept();
}

void XWindow::shadeWindow()
{
	if( _isShaded )
	{
		XLibWrapper::showWindow( _window );
		setGeometry( _x, _y, _width, _height+TITLE_SIZE+BOTTOM_BORDER_SIZE );
		_shade->setArrowType( Qt::DownArrow );
		_isShaded = false;
	}
	else
	{
		XLibWrapper::hideWindow( _window );
		setGeometry( _x, _y, _width, TITLE_SIZE+BOTTOM_BORDER_SIZE );
		_shade->setArrowType( Qt::RightArrow );
		_isShaded = true;
	}
}

void XWindow::minimizeWindow()
{
	XLibWrapper::hideWindow( _window );
	XLibWrapper::hideWindow( winId() );

	_isMinimized = true;
}

void XWindow::unMinimizeWindow()
{
	raise();
	_isMinimized = false;
}

void XWindow::maximizeWindow()
{
	if( _isMaximized )
	{
		unMaximizeWindow();
		return;
	}

	int x = 0;
	int y = 0;
	int width = XLibWrapper::getScreenWidth( XLibWrapper::getScreenForWindow(winId()) );
	int height = XLibWrapper::getScreenHeight( XLibWrapper::getScreenForWindow(winId()) );
	
	setGeometry( x, y, width, height-40 );

	XLibWrapper::moveWindow( _window, x, y+TITLE_SIZE );
	XLibWrapper::resizeWindow( _window, width, height-TITLE_SIZE-BOTTOM_BORDER_SIZE-40-2 );
	
	_isMaximized = true;
}

void XWindow::unMaximizeWindow()
{
	_isMaximized = false;
	applyProperties();
}

void XWindow::closeWindow()
{
	XDestroyWindow( XLibWrapper::getDisplay(), _window );
}

void XWindow::raise()
{
	show();
	focus();
	XLibWrapper::bringWindowOnTop( winId() );
	onTopWindow = this;
}

