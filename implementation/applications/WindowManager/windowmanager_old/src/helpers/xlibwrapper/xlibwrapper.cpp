
// local host
#include "xlibwrapper.h"

// STL headers
#include <list>

Display* XLibWrapper::_display = 0;


//##############################################################################################

void XLibWrapper::setDisplay( Display* display )
{
	_display = display;

	X11Agreements::ICCC::initAtoms( _display );
	X11Agreements::EWMH::initAtoms( _display );
}

Display* XLibWrapper::getDisplay()
{
	return _display;
}

//##############################################################################################

std::string XLibWrapper::getWindowTitleText( Window window )
{
	std::string titleText = X11Agreements::EWMH::getWindowTitleText( _display, window );
	if( 0 == titleText.length() )
		titleText = X11Agreements::ICCC::getWindowTitleText( _display, window );

	return titleText;
}

std::string XLibWrapper::getWindowTitleTextWhenIconified( Window window )
{
	std::string titleText = X11Agreements::EWMH::getWindowTitleTextWhenIconified( _display, window );
	if( 0 == titleText.length() )
		titleText = X11Agreements::ICCC::getWindowTitleTextWhenIconified( _display, window );

	return titleText;
}

//##############################################################################################

void XLibWrapper::getWindowGeometry( Window window, int& x, int& y, int& w, int& h )
{
	X11Agreements::ICCC::getWindowGeometry( _display, window, x, y, w, h );
}

void XLibWrapper::getWindowGeometryConstraints( Window window, int& minWidth, int& minHeight, int& maxWidth, int& maxHeight )
{
	X11Agreements::ICCC::getWindowGeometryConstraints( _display, window, minWidth, minHeight, maxWidth, maxHeight );
}

//##############################################################################################

bool XLibWrapper::isWindowSupposedTohaveDecorations( Window window )
{
	return ( X11Agreements::ICCC::isWindowSupposedTohaveDecorations(_display,window) || X11Agreements::EWMH::isWindowSupposedTohaveDecorations(_display,window) );
}

//##############################################################################################

void XLibWrapper::focusWindow( Window window )
{
	XSetInputFocus( _display, window, RevertToPointerRoot, CurrentTime);
}

void XLibWrapper::hideWindow( Window window )
{
	XUnmapWindow( _display, window );

	X11Agreements::ICCC::setWindowState( _display, window, WithdrawnState );
}

void XLibWrapper::showWindow( Window window )
{
	XMapWindow( _display, window );

	X11Agreements::ICCC::setWindowState( _display, window, NormalState );
}

void XLibWrapper::bringWindowOnTop( Window window )
{
	XRaiseWindow( _display, window );
}

void XLibWrapper::sendWindowToBack( Window window )
{
	XLowerWindow( _display, window );
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

void XLibWrapper::configureWindow( XConfigureRequestEvent* event )
{
	Window window = event->window;

	XWindowChanges windowChanges;
	windowChanges.width = event->width;
	windowChanges.height = event->height;
	windowChanges.border_width = 0;
	event->value_mask |= CWBorderWidth;

	XConfigureWindow( _display, window, event->value_mask, &windowChanges );
}

//##############################################################################################

std::list<Window> XLibWrapper::getWindowList( Window rootWindow )
{
	Window returnRootWindow;
	Window returnParentWindow;
	Window* childrenList;
	unsigned int childrenCount;

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
}

//##############################################################################################

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

