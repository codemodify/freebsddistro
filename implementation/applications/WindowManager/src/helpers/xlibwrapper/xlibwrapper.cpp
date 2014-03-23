
// local host
#include "xlibwrapper.h"

// STL headers
#include <list>

#include <X11/Xutil.h>

Display* g_display = 0;

//##############################################################################################
bool XLibWrapper::openX11Connection( char* displayName )
{
	g_display = XOpenDisplay( displayName ? displayName : ":0.0" );

	if( NULL != g_display )
		return false;

	setDisplay( g_display );

	return true;
}

void XLibWrapper::setDisplay( Display* display )
{
	g_display = display;

	// init the atoms for supported X11 agreements
	X11Agreements::ICCC::initAtoms( g_display );
	X11Agreements::EWMH::initAtoms( g_display );
}

Display* XLibWrapper::getDisplay()
{
	return g_display;
}

//##############################################################################################

bool XLibWrapper::createWindow( Window& window )
{
	window = 0;

	Window rootWindow = XDefaultRootWindow( g_display );
	window = XCreateWindow( g_display, rootWindow, 100, 100, 300, 200, 0, 0, InputOutput, CopyFromParent, 0, 0 );

	return ( 0 != window );
}

//##############################################################################################

void XLibWrapper::setWindowTitleText( Window window, std::string& title )
{
	X11Agreements::ICCC::setWindowTitleText( g_display, window, title );

	X11Agreements::EWMH::setWindowTitleText( g_display, window, title );
}

void XLibWrapper::getWindowTitleText( Window window, std::string& title )
{
	X11Agreements::EWMH::getWindowTitleText( g_display, window, title );

	if( 0 == title.length() )
		X11Agreements::ICCC::getWindowTitleText( g_display, window, title );
}

void XLibWrapper::getWindowTitleTextWhenIconified( Window window, std::string& title )
{
	X11Agreements::EWMH::getWindowTitleTextWhenIconified( g_display, window, title );

	if( 0 == title.length() )
		X11Agreements::ICCC::getWindowTitleTextWhenIconified( g_display, window, title );

}

//##############################################################################################

void XLibWrapper::getWindowGeometry( Window window, int& x, int& y, int& w, int& h )
{
	X11Agreements::ICCC::getWindowGeometry( g_display, window, x, y, w, h );
}

void XLibWrapper::getWindowGeometryConstraints( Window window, int& minWidth, int& minHeight, int& maxWidth, int& maxHeight )
{
	X11Agreements::ICCC::getWindowGeometryConstraints( g_display, window, minWidth, minHeight, maxWidth, maxHeight );
}

//##############################################################################################

bool XLibWrapper::isWindowSupposedTohaveDecorations( Window window )
{
	return ( 
		 X11Agreements::ICCC::isWindowSupposedTohaveDecorations(g_display,window) || 
		 X11Agreements::EWMH::isWindowSupposedTohaveDecorations(g_display,window) 
		);
}

//##############################################################################################

void XLibWrapper::focusWindow( Window window )
{
	XSetInputFocus( g_display, window, RevertToPointerRoot, CurrentTime);
}

void XLibWrapper::hideWindow( Window window )
{
	XUnmapWindow( g_display, window );

	X11Agreements::ICCC::setWindowState( g_display, window, WithdrawnState );
}

void XLibWrapper::showWindow( Window window )
{
	XMapWindow( g_display, window );

	X11Agreements::ICCC::setWindowState( g_display, window, NormalState );
}

void XLibWrapper::bringWindowOnTop( Window window )
{
	XRaiseWindow( g_display, window );
}

void XLibWrapper::sendWindowToBack( Window window )
{
	XLowerWindow( g_display, window );
}

void XLibWrapper::resizeWindow( Window window, int width, int height )
{
	XResizeWindow( g_display, window, width, height );
}

void XLibWrapper::moveWindow( Window window, int x, int y )
{
	XMoveWindow( g_display, window, x, y);
}

void XLibWrapper::destroyWindow( Window window )
{
	XDestroyWindow( g_display, window );
}

void XLibWrapper::killWindow( Window window )
{
	XKillClient( g_display, window );
}

void XLibWrapper::configureWindow( XConfigureRequestEvent* event )
{
	Window window = event->window;

	XWindowChanges windowChanges;
	windowChanges.x = event->x;
	windowChanges.y = event->y;
	windowChanges.width = event->width;
	windowChanges.height = event->height;
	windowChanges.sibling = event->above;
	windowChanges.stack_mode = event->detail;
	event->value_mask |= CWBorderWidth;

	XConfigureWindow( g_display, window, event->value_mask, &windowChanges );
}

//##############################################################################################

void XLibWrapper::getWindowList( Window rootWindow, WindowList& windowList )
{
	Window returnRootWindow;
	Window returnParentWindow;
	Window* childrenList;
	unsigned int childrenCount;

	XQueryTree( g_display, rootWindow, &returnRootWindow, &returnParentWindow, &childrenList, &childrenCount );

	// here we put in the list only windows that do not have the 'override-redirect' set
	XWindowAttributes windowAttributes;
	for( unsigned int index = 0; index < childrenCount; ++index )
	{
		Window window = childrenList[index];

		XGetWindowAttributes( g_display, window, &windowAttributes );
		if( (False==windowAttributes.override_redirect) && (IsViewable==windowAttributes.map_state) )
			windowList.push_back( window );
	}

	XFree( childrenList );
}

//##############################################################################################

int XLibWrapper::getScreenWidth( int screen )
{
	return XDisplayWidth( g_display, screen );
}

int XLibWrapper::getScreenHeight( int screen )
{
	
	return XDisplayHeight( g_display, screen );
}

int XLibWrapper::getScreenForWindow( Window window )
{
	XWindowAttributes windowAttributes;
	XGetWindowAttributes( g_display, window, &windowAttributes );

	return XScreenNumberOfScreen( windowAttributes.screen );
}

