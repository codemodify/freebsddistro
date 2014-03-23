
// local headers
#include "resizeframe.h"
#include "client/client.h"
#include "fns.h"

ResizeFrame::ResizeFrame( Client* parent, const char* name ):
	QLabel(parent,name)
{
	setFrameStyle( QFrame::Box | QFrame::Raised );
	_client = parent;

}

void ResizeFrame::mouseReleaseEvent( QMouseEvent* mouseEvent )
{
	releaseMouse();
}

void ResizeFrame::mousePressEvent( QMouseEvent* mouseEvent )
{
	resizedrag(c);
	sendconfig(c);
}

void ResizeFrame::mouseMoveEvent( QMouseEvent* mouseEvent )
{

}

