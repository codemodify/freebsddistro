#ifndef BALOON_NOTIFICATION_MANAGER_H_
#define BALOON_NOTIFICATION_MANAGER_H_

// Qt's headers
#include <QtCore/QQueue>
#include <QtCore/QTimer>

// local headers
#include "baloonnotification/baloonnotification.h"

class QString;

/*
 * 
 * This class implements a Baloon-Notification-Manager.
 * The idea is to have max 3 baloons visible at one moment of time,
 * the rest baloons (if there are some) are waiting in a queue for later pop-up.
 * 
 * Usage:
 * BaloonNotificationManager::showBaloonNotification( "This is a info", Info ); 
 * 
 */

class BaloonNotificationManager : QObject
{	
	// using Qt's signal/slots framework
	Q_OBJECT
	
public:
	~BaloonNotificationManager();
	
	// pop-ups a baloon-notification
	static void showBaloonNotification( QString notificationMessage, NotificationType notificationType = Info );
	
	// stops all the running baloons and frees all used resources, usually used at exit to free all used resources
	static void cleanUp();
	
	// blocks untill all current baloons will not finish
	static void waitForBaloonsToFinish();
	
	// sets the desired baloon's notification position on the screen
	static void setBaloonNotificationPosition( NotificationPosition notificationPosition );
	
private slots:

	// receives a notification that a baloon is done and disappeared
	void baloonFinished( BaloonNotification* baloonNotification );
	
	// starts the checker loop for the baloon-manager
	void checkForBaloons();	
	
private:

	// disabling the direct creation of the object
	BaloonNotificationManager();
	
	// singleton pattern implementation
	static BaloonNotificationManager* s_baloonNotificationManager;
	
	// used to let user to setup a default notification-position for baloons
	static NotificationPosition s_notificationPosition;
	
	// the queue of waiting messages to be poped-up
	struct Notification{ QString notificationMessage; NotificationType notificationType; };
	QQueue<Notification> m_waitingNotifications;
	
	// stores the current free positions
	BaloonNotification* m_freePosition[3];
	
	QTimer m_timer;
};

#endif
