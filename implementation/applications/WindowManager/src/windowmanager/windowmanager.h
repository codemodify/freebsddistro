#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H


// Qt's headers
#include <QtGui/QApplication>
#include <QtCore/QMap>
#include <QtGui/QToolBar>
#include <QtCore/QPointer>

// local headers
#include "helpers/xlibwrapper/xlibwrapper.h"

class XWindow;
class TaskBar;



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

public:
	WindowManager( int argc, char* argv[] );
	~WindowManager();
	
	bool initEnvironment();

	

private: // override from base class, to have direct access to the X11 events to be able to do our job 
	bool x11EventFilter( XEvent* event );



private: // X11 related global stuff.
	QList<Window> _rootWindows;



private:
	QMap< Window, XWindow* > _theKnowlegeBase;
	TaskBar* _taskBar;


private: // these are the direct duties of a self-respecting window-manager

	// window circulation is bring-on-top or send-to-back operation.
	// it makes sense to do such operations only on the direct children of the root-window
	void handleCirculateRequest( XCirculateRequestEvent* event );

	// because as a window-manager we act as a gateway for all the messages that go toward the root-window,
	// we'll have to handle the "ConfigureRequest" events from both clients that we handle and those that we don't.
	// for the client we handle we configure the original Window and make some adjustments to the wrapper frame.
	// for the client that we don't handle we just configure it.
	void handleConfigureRequest( XConfigureRequestEvent* event );

	// a client will ask for mapping(asks to be exposed to the flow of X11 requests/events/notification) if:
	// - is a an existing window - we handle this easily because this window wants to be restored from the minimized state
	// - is a new window - we handle it and make it to be in our database
	void handleMapRequest( XMapRequestEvent* event );


private: // these are the second direct duties for a window-manager

	void handleDestroyNotification( XDestroyWindowEvent* event );
	void handleClientMessage( XClientMessageEvent* event );
	void handlePropertyChangedNotification( XPropertyEvent* event );
	void handleEnterNotification( XCrossingEvent* event );
	void handleKeyRelease( XKeyReleasedEvent* event );
	void handleButtonRelease( XButtonReleasedEvent* event );

};

#endif

