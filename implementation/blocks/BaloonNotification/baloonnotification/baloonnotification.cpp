
// Qt's headers
#include <QtCore/QTimer>
#include <QtCore/QSize>
#include <QtCore/QRect>
#include <QtGui/QDesktopWidget>

// local headers
#include "baloonnotification.h"
#include "General"

BaloonNotification::BaloonNotification()
{
}

BaloonNotification::~BaloonNotification()
{
}

void BaloonNotification::initEnvironement( QString notificationMessage,
										NotificationType notificationType,
										NotificationPosition notificationPosition,
										NotificationOrder notificationOrder )
{
	setupUi( this );
	
	// applying the attributes for this notification
	m_text->setText( notificationMessage );
	m_icon->setPixmap( getPixmapForThisNotification(notificationType) );
	//setWindowFlags( Qt::ToolTip );
	move( getTheCoordinatesToAppearAt(notificationPosition,notificationOrder) );
	show();
	connect( this, SIGNAL(accepted()), this, SLOT(balooFinished()) );
	QTimer::singleShot( 5000, this, SLOT(accept()) );
}

QPoint BaloonNotification::getTheCoordinatesToAppearAt( NotificationPosition notificationPosition, NotificationOrder notificationOrder )
{
	// setting the position on the screen to appear
	QDesktopWidget desktopWidget;

	QPoint position;
	
	position.setX( desktopWidget.geometry().width() - size().width() );
	int height = size().height();
	
	switch( notificationPosition )
	{
		case TopRight:
			height = height * notificationOrder;
			break;
			
		case BottomRight:
			height = desktopWidget.geometry().height() - height * notificationOrder - height;
			break;
	}
	
	position.setY( height );
	
	return position;
}

void BaloonNotification::balooFinished()
{
	emit timeOut( this );
	disconnect();
	deleteLater();
}

QPixmap BaloonNotification::getPixmapForThisNotification( NotificationType notificationType )
{
	// getting the settings-path for current application
	QString pathToBaloons = getFullPathForSettingsFolder();
	
	switch( notificationType )
	{
		case Info:
			return QPixmap( pathToBaloons + "/baloonInfo.png" );
			break;
			
		case Error:
			return QPixmap( pathToBaloons + "/baloonError.png" );
			break;
			
		case Warning:
			return QPixmap( pathToBaloons + "/baloonWarning.png" );
			break;
			
		case Note:
			return QPixmap( pathToBaloons + "/baloonNote.png" );
			break;
			
		default:
			break;
	}
	
	return QPixmap();
}
