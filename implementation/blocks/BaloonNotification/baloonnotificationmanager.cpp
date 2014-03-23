
// Qt's headers
#include <QtCore/QSemaphore>

// local headers
#include "baloonnotificationmanager.h"

BaloonNotificationManager* BaloonNotificationManager::s_baloonNotificationManager = 0;

NotificationPosition BaloonNotificationManager::s_notificationPosition = TopRight;

// used to sync the access to the "s_baloonNotificationManager" resource
QSemaphore g_semaphoreForManager(1);

// used to sync the access to the "s_baloonNotificationManager" resource
QSemaphore g_semaphoreForNotifications(1);

BaloonNotificationManager::BaloonNotificationManager()
{
	m_freePosition[0] = 0;
	m_freePosition[1] = 0;
	m_freePosition[2] = 0;
	m_timer.setInterval( 1000 );
}

BaloonNotificationManager::~BaloonNotificationManager()
{
}

void BaloonNotificationManager::showBaloonNotification( QString notificationMessage, NotificationType notificationType )
{
	g_semaphoreForManager.acquire();
	
	// checking for a valid instance
	if( 0 == s_baloonNotificationManager )
	{
		s_baloonNotificationManager = new BaloonNotificationManager();
		connect( &s_baloonNotificationManager->m_timer, SIGNAL(timeout()), s_baloonNotificationManager, SLOT(checkForBaloons()) );
	}
	
	s_baloonNotificationManager->m_timer.start();
	
	// adding a new 'notification' to the waiting list	
	Notification notification;
	notification.notificationMessage = notificationMessage;
	notification.notificationType = notificationType;
	
	g_semaphoreForNotifications.acquire();
	s_baloonNotificationManager->m_waitingNotifications.enqueue( notification );
	g_semaphoreForNotifications.release();
	
	g_semaphoreForManager.release();
}

void BaloonNotificationManager::cleanUp()
{
	if( 0 == s_baloonNotificationManager )
		return;
	
	s_baloonNotificationManager->m_timer.stop();
	s_baloonNotificationManager->disconnect();
	s_baloonNotificationManager->deleteLater();
	s_baloonNotificationManager = 0;
	
	g_semaphoreForManager.release();
	g_semaphoreForNotifications.release();	
}

void BaloonNotificationManager::checkForBaloons()
{
	// checking for how much baloons we are running at this moment
	for( int order = First; order <= Third; ++order )
	{
		bool haveToStop = m_waitingNotifications.isEmpty();
		
		if( (0==m_freePosition[order]) && !haveToStop )
		{
			g_semaphoreForNotifications.acquire();
			
			Notification notification = m_waitingNotifications.dequeue();
			m_freePosition[order] = new BaloonNotification();
			m_freePosition[order]->initEnvironement( notification.notificationMessage, notification.notificationType, s_notificationPosition, static_cast<NotificationOrder>(order) );
			connect( m_freePosition[order], SIGNAL(timeOut(BaloonNotification*)), this, SLOT(baloonFinished(BaloonNotification*)) );
			g_semaphoreForNotifications.release();
		}
	}
}

void BaloonNotificationManager::baloonFinished( BaloonNotification* baloonNotification )
{
	g_semaphoreForNotifications.acquire();
	
	// registering a free position for a new notification baloon
	for( int index = 0; index<3; ++index )
	{
		if( baloonNotification == m_freePosition[index] )
		{
			m_freePosition[index] = 0;
			break;
		}
	}
	
	g_semaphoreForNotifications.release();
}

void BaloonNotificationManager::waitForBaloonsToFinish()
{
	bool result = true;
	qApp->processEvents();
	
	do
	{
		qApp->processEvents();
		g_semaphoreForNotifications.acquire();
		
		result = s_baloonNotificationManager->m_freePosition[0]||
				s_baloonNotificationManager->m_freePosition[1]||
				s_baloonNotificationManager->m_freePosition[2]||
				(!s_baloonNotificationManager->m_waitingNotifications.isEmpty());
		g_semaphoreForNotifications.release();
	}while( result );
}
