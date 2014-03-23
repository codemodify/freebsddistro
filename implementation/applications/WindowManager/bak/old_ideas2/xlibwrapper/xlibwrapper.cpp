
// local includes
#include "xlibwrapper.h"

// STL headers
#include <list>

Display* XLibWrapper::_display = 0;

std::string XLibWrapper::getWindowName( Window window )
{
	char* buffer;
	if( !XFetchName(_display,window,&buffer) )
		return std::string().c_str();

	std::string windowName(buffer);
	XFree( buffer );

	return windowName;
}

void XLibWrapper::focusWindow( Window window )
{
	bringWindowOnTop( window );
	XSetInputFocus( _display, window, RevertToPointerRoot, CurrentTime);
}
void XLibWrapper::hideWindow( Window window )
{
	XUnmapWindow( _display, window );
}

void XLibWrapper::showWindow( Window window )
{
	XMapWindow( _display, window );
}

void XLibWrapper::bringWindowOnTop( Window window )
{
	XMapRaised( _display, window );
}

void XLibWrapper::sendWindowToBack( Window window )
{
	XLowerWindow( _display, window );
}

void XLibWrapper::getWindowGeometry( Window window, int& x, int& y, int& w, int& h )
{
	XWindowAttributes windowAttributes;
	XGetWindowAttributes( _display, window, &windowAttributes );
	x = windowAttributes.x;
	y = windowAttributes.y;
	w = windowAttributes.width;
	h = windowAttributes.height;
}

void XLibWrapper::resizeWindow( Window window, int width, int height )
{
	XResizeWindow( _display, window, width, height );
}

void XLibWrapper::moveWindow( Window window, int x, int y )
{
	XMoveWindow( _display, window, x, y);
}

void XLibWrapper::destroyWindow( Window window )
{
	XDestroyWindow( _display, window );
}

void XLibWrapper::killWindow( Window window )
{
	XKillClient( _display, window );
}

/*std::list XLibWrapper::getWindowList( Window rootWindow )
{
	Window returnRootWindow;
	Window returnParentWindow;
	Window* childrenList;
	int childrenCount;

	std::list<Window> windowList;

	XQueryTree( _display, rootWindow, &returnRootWindow, &returnParentWindow, &childrenList, &childrenCount );

	// here we put in the list only windows that do not have the 'override-redirect' set
	XWindowAttributes windowAttributes;
	for( int index = 0; index < childrenCount; ++index )
	{
		Window window = childrenList[index];

		XGetWindowAttributes( _display, window, &windowAttributes );
		if( (False==windowAttributes.override_redirect) && (IsViewable==windowAttributes.map_state) )
			windowList.push_back( window );
	}

	XFree( childrenList );

	return windowList;
}*/

int XLibWrapper::getScreenWidth( int screen )
{
	return XDisplayWidth( _display, screen );
}

int XLibWrapper::getScreenHeight( int screen )
{
	
	return XDisplayHeight( _display, screen );
}

int XLibWrapper::getScreenForWindow( Window window )
{
	XWindowAttributes windowAttributes;
	XGetWindowAttributes( _display, window, &windowAttributes );

	return XScreenNumberOfScreen( windowAttributes.screen );
}

void XLibWrapper::setDisplay( Display* display )
{
	_display = display;
}

Display* XLibWrapper::getDisplay()
{
	return _display;
}
