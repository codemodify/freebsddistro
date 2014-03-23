#ifndef BALOON_NOTIFICATION_H_
#define BALOON_NOTIFICATION_H_

#include <QtCore/QString>
#include <QtGui/QDialog>
#include <QtCore/QPoint>
#include <QtGui/QPixmap>

#include "ui_baloonnotification.h"

typedef enum NotificationType{ Info = 0, Error, Warning, Note };

typedef enum NotificationPosition{ TopRight = 0, BottomRight };

typedef enum NotificationOrder{ First = 0, Second, Third };

class BaloonNotification : public QDialog, public Ui::BaloonNotification
{
	Q_OBJECT
	
public:
	BaloonNotification();

	~BaloonNotification();
	
	void initEnvironement(	QString notificationMessage,
							NotificationType notificationType,
							NotificationPosition notificationPosition,
							NotificationOrder notificationOrder  );
							
	void run();
	
signals:
	void timeOut(BaloonNotification*);
	
private slots:
	void balooFinished();
	
private:
	QPoint getTheCoordinatesToAppearAt( NotificationPosition notificationPosition, NotificationOrder notificationOrder );
	QPixmap getPixmapForThisNotification( NotificationType notificationType );
};

#endif
