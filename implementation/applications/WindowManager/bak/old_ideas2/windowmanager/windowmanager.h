#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H


// Qt's headers
#include <QtGui/QApplication>
#include <QtCore/QMap>
#include <QtGui/QToolBar>
#include <QtCore/QPointer>

// local headers
#include "xwindow/xwindow.h"




/*********************************************************************************************
 *
 *
 *	This class implements a X11 Window Manager.
 *
 *
 *	No external plugin support. Why not: 
 *	- usually this feature implies aditional resources usage for a design that do not prove to worth it,
 *		because we have to work in an abstract mode to allow plugins to interact with us.
 *	- damage to robustness and to my "Workspace" concept.
 *	- it is supposed that it already provides everything you need, no 
 *		aditional applications required to help you to work.
 *
 *
 *********************************************************************************************/
class WindowManager : public QApplication
{
	Q_OBJECT

public:
	WindowManager( int argc, char* argv[] );
	~WindowManager();
	
	bool initEnvironment();

	

private: // override from base class, to have direct access to the X11 events to be able to do our job 
	bool x11EventFilter( XEvent* event );



private: // X11 related global stuff. This is our channel to talk to X11 server
	Display* _x11Display;



private:
	QMap<Window,XWindow*> _windowList;


	void handleButtonPressEvent( XButtonPressedEvent* buttonPressedEevent );
	void handleButtonReleaseEvent( XButtonReleasedEvent* buttonReleasedEvent );

	void handleMapRequestEvent( XMapRequestEvent* mapRequestEvent );
	void handleUnmapEvent( XUnmapEvent* unMapEvent );

	void handleConfigureRequestEvent( XConfigureRequestEvent* configureRequestEvent );

	void handleCirculateRequestEvent( XCirculateRequestEvent* circulateRequestEvent );

	void handleDestroyEvent( XDestroyWindowEvent* destroyEvent );

	void handleClientMessageEvent( XClientMessageEvent* clientMessageEvent );

	void handlePropertyChangeEvent( XPropertyEvent* propertyEvent );

	void handleEnterEvent( XCrossingEvent* crossingEvent );

	void handleColorMapChangeEvent( XColormapEvent* colorMapEvent );

	void handleExposeEvent( XExposeEvent* exposeEvent );

};

#endif

