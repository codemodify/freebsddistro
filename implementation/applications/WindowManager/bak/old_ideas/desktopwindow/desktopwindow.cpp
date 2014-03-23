#include "desktopwindow.h"

DesktopWindow::DesktopWindow( Window window ):
	QObject(0),
	_x11WindowId( window )
	_titleText(""),
	_isMaximized(false),
	_isMinized(false),
	_isShaded(false),
	_size(0,0),
	_isOntop(false)
{
}

DesktopWindow::~DesktopWindow()
{
}

void DesktopWindow::setTitleText( QString titleText )
{
	_titleText = titleText;
	
	// Xlib calls
}

void DesktopWindow::maximize()
{
	_isMaximized = true;
	
	// Xlib calls
}

void DesktopWindow::unmaximize()
{
	_isMaximized = false;
	
	// Xlib calls
}

void DesktopWindow::minimize()
{
	_isMinimized = true;
	
	// Xlib calls
	XIconifyWindow( _x11WindowId, 
}

void DesktopWindow::unminimize()
{
	_isMinimized = false;
	
	// Xlib calls
}

void DesktopWindow::sahde()
{
	_isShaded = true;
	
	// Xlib calls
}

void DesktopWindow::unshade()
{
	_isShaded = false;
	
	// Xlib calls	
}

void DesktopWindow::setSize( QSize size )
{
	_size = size;
	
	// Xlib calls
}

void DesktopWindow::setOntop( bool ontop )
{
	_isOntop = ontop;
	
	// Xlib calls
}

