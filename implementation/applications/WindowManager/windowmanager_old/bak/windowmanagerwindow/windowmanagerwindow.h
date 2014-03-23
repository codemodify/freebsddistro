#ifndef WINDOWMANAGER_WINDOW_H
#define WINDOWMANAGER_WINDOW_H

// Qt's headers
#include <QtCore/QObject>
#include <QtCore/QPointer>

// local headers
#include "titlebar/titlebar.h"
#include "bottombar/bottombar.h"
#include "xlibwrapper/xlibwrapper.h"


class WindowManagerWindow : public QObject
{

	Q_OBJECT

public:
	WindowManagerWindow( Window window );
	~WindowManagerWindow();

	QString getText(){ return _titleBar->getTitlebarText(); }



public slots:
	void minimize();
	void maximize();
	void close();
	void move( int globalX, int globalY );

	void resizeLeft( int globalX, int globalY );
	void resizeRight( int globalX, int globalY );

	void unMinimize();
	bool isMinimized();


private:
	QPointer<TitleBar> _titleBar;
	Window _x11Window;
	QPointer<BottomBar> _bottomBar;

	int _normalX, _normalY, _normalWidth, _normalHeight;
	int _maximizedX, _maximizedY, _maximizedWidth, _maximizedHeight;

	enum { Maximized, Normal, Minimized, Shaded } _previousState, _state;

};

#endif

