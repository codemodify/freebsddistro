#ifndef XWINDOW_H
#define XWINDOW_H

#include <QtGui/QDockWidget>
#include <QtGui/QDesktopWidget>
#include <QtGui/QVBoxLayout>

#include "xlibwrapper/xlibwrapper.h"

class XWindow : public QDockWidget
{

public:
	XWindow(Window w);
	~XWindow();
};
#endif
