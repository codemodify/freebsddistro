
// Qt headers
#include <QtGui/QX11Info>

// local headers
#include "gui.h"
#include "xlibwrapper.h"

Gui::Gui()
{
	setupUi( this );

	connect( _setDisplayButton, SIGNAL(clicked(bool)), this, SLOT(setDisplay()) );
	connect( _createdWindowButton, SIGNAL(clicked(bool)), this, SLOT(createWindow()) );
	connect( _showWindowButton, SIGNAL(clicked(bool)), this, SLOT(showWindow()) );
	connect( _setTitleTextButton, SIGNAL(clicked(bool)), this, SLOT(setWindowTitleText()) );
	connect( _getTitleTextButton, SIGNAL(clicked(bool)), this, SLOT(getWindowTitleText()) );
	connect( _hideWindowButton, SIGNAL(clicked(bool)), this, SLOT(hideWindow()) );
	connect( _focusWindowButton, SIGNAL(clicked(bool)), this, SLOT(focusWindow()) );
	connect( _bringWindowOnTopButton, SIGNAL(clicked(bool)), this, SLOT(bringWindowOnTop()) );
	connect( _sendWindowToBackButton, SIGNAL(clicked(bool)), this, SLOT(sendWindowToBack()) );

}

void Gui::setDisplay()
{
	Display* display = QX11Info::display();
	XLibWrapper::setDisplay( display );

	_display->setText( "display set" );
}

void Gui::createWindow()
{
	Window window;
	if( false == XLibWrapper::createWindow(window) )
		return;

	_createdWindow->setText( QString("%1").arg(window) );
	_windowToShow->setText( QString("%1").arg(window) );
	_windowToSetTitleText->setText( QString("%1").arg(window) );
	_windowToGetTitleText->setText( QString("%1").arg(window) );

	_windowToHide->setText( QString("%1").arg(window) );
	_windowToFocus->setText( QString("%1").arg(window) );
	_windowToBring->setText( QString("%1").arg(window) );
	_windowToSend->setText( QString("%1").arg(window) );
}

void Gui::showWindow()
{
	XLibWrapper::showWindow( _windowToShow->text().toLong() );
}

void Gui::setWindowTitleText()
{
	std::string title;
	title.assign( _titleTextToSet->text().toAscii().data() );

	long window = _windowToSetTitleText->text().toLong();

	XLibWrapper::setWindowTitleText( window, title );
}

void Gui::getWindowTitleText()
{
	std::string title;

	long window = _windowToGetTitleText->text().toLong();

	XLibWrapper::getWindowTitleText( window, title );

	_titleText->setText( title.c_str() );
}

void Gui::hideWindow()
{
	long window = _windowToHide->text().toLong();

	XLibWrapper::hideWindow( window );
}

void Gui::focusWindow()
{
	XLibWrapper::focusWindow( _windowToFocus->text().toLong() );
}

void Gui::bringWindowOnTop()
{
	XLibWrapper::bringWindowOnTop( _windowToBring->text().toLong() );
}

void Gui::sendWindowToBack()
{
	XLibWrapper::sendWindowToBack( _windowToSend->text().toLong() );
}

