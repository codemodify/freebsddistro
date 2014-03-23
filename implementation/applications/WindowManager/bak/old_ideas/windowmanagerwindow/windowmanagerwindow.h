#ifndef WINDOWMANAGER_WINDOW_H
#define WINDOWMANAGER_WINDOW_H

// Qt's headers
#include <QtGui/QDockWidget>
#include <QtCore/QPointer>

// local headers
#include "xlibwrapper/xlibwrapper.h"


class WindowManagerWindow : public QDockWidget
{

	Q_OBJECT

public:
	WindowManagerWindow( Window window );
	~WindowManagerWindow();

	QString getText(){ return windowTitle(); }



protected:
	void closeEvent( QCloseEvent* event );


private:
	Window _x11Window;

};

#endif

