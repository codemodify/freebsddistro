#ifndef X_LIB_WRAPPER_H
#define X_LIB_WRAPPER_H

// STL headers
#include <string>

// local includes
#include "x11agreements/x11agreements.h"


/****************************************************************************
*
*	This namespace is a wrapper for the Xlib library.
*
*
*
*
*	Usage 1: - use wrapper just to get info about X11's resources, or use its funtionality
*
*	XLibWrapper::setDisplay( display ); // required because we do not call the "openX11Connection()"
*
*	Window window;
*	XLibWrapper::createWindow( window );
*	XLibWrapper::bringWindowOnTop( window );
*
*
*
*
*	Usage 2: - use wrapper to connect to X11 and get info about X11's resources, or use its funtionality
*
*	XLibWrapper::openX11Connection(); // required because we do not call the "setDisplay()"
*
*	Window window;
*	XLibWrapper::createWindow( window );
*	XLibWrapper::bringWindowOnTop( window );
*
****************************************************************************/

namespace XLibWrapper
{

	// X11-server communication channel related
	bool openX11Connection( char* displayName=0 );
	void setDisplay( Display* display );
	Display* getDisplay();


	// window related
	bool createWindow( Window& window );

	void setWindowTitleText( Window window, std::string& title );
	void getWindowTitleText( Window window, std::string& title );
	void getWindowTitleTextWhenIconified( Window window, std::string& title );

	void focusWindow( Window window );
	void hideWindow( Window window );
	void showWindow( Window window );
	void bringWindowOnTop( Window window );
	void sendWindowToBack( Window window );

	void getWindowGeometry( Window window, int& x, int& y, int& w, int& h );
	void getWindowGeometryConstraints( Window window, int& minWidth, int& minHeight, int& maxWidth, int& maxHeight );
	void resizeWindow( Window window, int width, int height );
	void moveWindow( Window window, int x, int y );

	void destroyWindow( Window window );
	void killWindow( Window window );

	void configureWindow( XConfigureRequestEvent* event );

	bool isWindowSupposedTohaveDecorations( Window window );

	typedef std::list<Window> WindowList;
	void getWindowList( Window rootWindow, WindowList& windowList );

	int getScreenWidth( int screen );
	int getScreenHeight( int screen );
	int getScreenForWindow( Window window );
};

#endif

