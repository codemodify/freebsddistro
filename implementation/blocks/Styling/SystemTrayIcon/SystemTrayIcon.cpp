
// Qt headers
#include <QtGui/QWidget>
#include <QtGui/QMenu>

// local headers
#include "SystemTrayIcon.h"

SystemTrayIcon::SystemTrayIcon( QWidget* widget, QMenu* menu ) :
	QSystemTrayIcon( widget ),
	_widget( widget ),
	_menu( menu )
{
	connect( this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
         		this, SLOT(sysTrayIconActivated(QSystemTrayIcon::ActivationReason)) );
}

SystemTrayIcon::~SystemTrayIcon()
{}

void SystemTrayIcon::sysTrayIconActivated( QSystemTrayIcon::ActivationReason activationReason )
{
	switch( activationReason )
	{
		case QSystemTrayIcon::Context:
         		_menu->exec( geometry().bottomLeft() );
	         break;

		default:
			_widget->show();
			break;
     }
}
