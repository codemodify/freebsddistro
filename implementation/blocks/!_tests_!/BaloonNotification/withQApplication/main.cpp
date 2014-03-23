#include <BaloonNotificationManager>
#include <QtGui/QApplication>

int main(int argc, char* argv[])
{
	QApplication qApplication( argc, argv );
	BaloonNotificationManager::showBaloonNotification("This test should succeed as Info", Info);
	BaloonNotificationManager::showBaloonNotification("This test should succeed as Error", Error);
	BaloonNotificationManager::showBaloonNotification("This test should succeed as Warning", Warning);
	BaloonNotificationManager::showBaloonNotification("This test should succeed as Note", Note);
	
	qApplication.exec();
	
	BaloonNotificationManager::waitForBaloonsToFinish();
	BaloonNotificationManager::cleanUp();
	
	return 0;
}
