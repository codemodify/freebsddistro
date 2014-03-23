
// local headers
#include "windowmanagerwindow.h"


WindowManagerWindow::WindowManagerWindow( Window window ):
	_x11Window( window )
{
	_previousState = _state = Normal;

	_normalX = _normalY = 0;
	_normalWidth = _normalHeight = 600;

	_maximizedX = _maximizedY = 0;
	_maximizedWidth = XLibWrapper::getScreenWidth( XLibWrapper::getScreenForWindow(_x11Window) );
	_maximizedHeight = XLibWrapper::getScreenHeight( XLibWrapper::getScreenForWindow(_x11Window) );

	// init the titlebar
	_titleBar = new TitleBar();
	std::string windowName = XLibWrapper::getWindowName( _x11Window );
	_titleBar->setTitlebarText( windowName.c_str() );
	
	connect( _titleBar, SIGNAL(minimizeButton()), this, SLOT(minimize()) );
	connect( _titleBar, SIGNAL(maximizeButton()), this, SLOT(maximize()) );
	connect( _titleBar, SIGNAL(closeButton()), this, SLOT(close()) );
	connect( _titleBar, SIGNAL(titleDragged(int,int)), this, SLOT(move(int,int)) );

	// init the bottombar
	_bottomBar = new BottomBar();
	connect( _bottomBar, SIGNAL(reizeLeft(int,int)), this, SLOT(resizeLeft(int,int)) );	
	connect( _bottomBar, SIGNAL(reizeRight(int,int)), this, SLOT(resizeRight(int,int)) );	
}

WindowManagerWindow::~WindowManagerWindow()
{
	XLibWrapper::destroyWindow( _x11Window );
}

void WindowManagerWindow::minimize()
{
	_titleBar->hide();
	_bottomBar->hide();
	XLibWrapper::hideWindow( _x11Window );

	_previousState = _state;
	_state = Minimized;
}

void WindowManagerWindow::maximize()
{
	_previousState = _state;

	switch( _state )
	{
		case Maximized: // here we have to switch to normal mode
		XLibWrapper::moveWindow( _x11Window, _maximizedX, _maximizedY );
		XLibWrapper::resizeWindow( _x11Window, _maximizedWidth, _maximizedHeight );
		_state = Normal;
		break;

		case Normal: // here we have to switch to maximized mode
		XLibWrapper::moveWindow( _x11Window, _normalX, _normalY );
		XLibWrapper::resizeWindow( _x11Window, _normalWidth, _normalHeight );
		_state = Maximized;
		break;

		default:
		break;
	};
	
}

void WindowManagerWindow::close()
{
	deleteLater();
}

void WindowManagerWindow::move( int globalX, int globalY )
{
	_normalX = globalX;
	_normalY = globalY;
	unMinimize();

}

void WindowManagerWindow::resizeLeft( int globalX, int globalY )
{
	globalX = globalY;
}

void WindowManagerWindow::resizeRight( int globalX, int globalY )
{
	globalX = globalY;
}

void WindowManagerWindow::unMinimize()
{
	_titleBar->show();
	_titleBar->setGeometry( _normalX, _normalY, _normalWidth, _normalHeight );

	XLibWrapper::showWindow( _x11Window );
	XLibWrapper::moveWindow( _x11Window, _normalX, _normalY+TITLE_HEIGHT );
	XLibWrapper::resizeWindow( _x11Window, _normalWidth, _normalHeight-BOTTOM_HEIGHT );
	XLibWrapper::focusWindow( _x11Window );

	_bottomBar->show();
	_bottomBar->setGeometry( _normalX, _normalHeight-BOTTOM_HEIGHT, _normalWidth, BOTTOM_HEIGHT );

	_previousState = _state;
	_state = Normal;
}

bool WindowManagerWindow::isMinimized()
{
	return ( Minimized == _state );
}

