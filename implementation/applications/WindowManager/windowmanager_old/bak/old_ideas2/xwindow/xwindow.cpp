

#include <QtGui/QApplication>
#include <QtGui/QX11Info>
#include "xwindow.h"
#include <QtGui/QX11EmbedContainer>
#include <QtGui/QVBoxLayout>

XWindow::XWindow(Window w) : QDockWidget() 
{
	setGeometry( 500, 500, 400, 300 );

    	/*/ overwrite Qt-defaults because we need SubstructureNotifyMask
	XSelectInput(QX11Info::display(), winId(),
  		 KeyPressMask | KeyReleaseMask |
  		 ButtonPressMask | ButtonReleaseMask |
  		 KeymapStateMask |
   		 ButtonMotionMask |
  		 PointerMotionMask |
		 EnterWindowMask | LeaveWindowMask |
  		 FocusChangeMask |
  		 ExposureMask |
		 StructureNotifyMask |
		 SubstructureRedirectMask |
		 SubstructureNotifyMask);*/

	//QX11EmbedContainer* cont = new QX11EmbedContainer();
	//setWidget( cont );

	//XMapWindow( QX11Info::display(), winId() );
	//XMapWindow( QX11Info::display(), w );

	//cont->embedClient( w );

}

XWindow::~XWindow()
{
}
