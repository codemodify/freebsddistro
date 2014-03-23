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

	Display* _x11Display; // This is our channel to talk to X11 server

	QList<Window> _rootWindows;

	void setRootWindowsMask();



private:
	QMap< Window, XWindow* > _theKnowlegeBase;
	TaskBar* _taskBar;

	void handleCirculateRequest( XCirculateRequestEvent* event );
	void handleConfigureRequest( XConfigureRequestEvent* event );
	void handleMapRequest( XMapRequestEvent* event );

	void handleDestroyNotification( XDestroyWindowEvent* event );
	void handleClientMessage( XClientMessageEvent* event );
	void handlePropertyChangedNotification( XPropertyEvent* event );
	void handleEnterNotification( XCrossingEvent* event );
	void handleKeyRelease( XKeyReleasedEvent* event );
	void handleButtonRelease( XButtonReleasedEvent* event );

};

#endif

