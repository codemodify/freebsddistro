#ifndef X_LIB_WRAPPER_H
#define X_LIB_WRAPPER_H

// X11 headers
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/X.h>
#include <X11/Xutil.h>
#include <X11/extensions/shape.h>

#include <string>

// local includes
#include "x11agreements/agreements.h"


/****************************************************************************
*
*	This namespace is a wrapper for the Xlib library.
*
*	Usage:
*
*	XLibWrapper::setDisplay( display ); // this has to be the first call before use of any other functions from this name space
*	std::string windowName = XLibWrapper::getWindowName( window );
*	XLibWrapper::bringWindowOnTop( window );
*
****************************************************************************/

namespace XLibWrapper
{
	// window related
	std::string getWindowTitleText( Window window );
	std::string getWindowTitleTextWhenIconified( Window window );

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

	std::list<Window> getWindowList( Window rootWindow );

	int getScreenWidth( int screen );
	int getScreenHeight( int screen );
	int getScreenForWindow( Window window );


	// X11-server communication channel related
	void setDisplay( Display* display );
	Display* getDisplay();
	extern Display* _display;
};

#endif
