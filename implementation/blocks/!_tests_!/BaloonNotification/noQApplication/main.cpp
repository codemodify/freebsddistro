#include <BaloonNotificationManager>

int main(int argc, char* argv[])
{
	BaloonNotificationManager::showBaloonNotification("This test should NOT succeed as Info", Info);
	BaloonNotificationManager::showBaloonNotification("This test should NOT succeed as Error", Error);
	BaloonNotificationManager::showBaloonNotification("This test should NOT succeed as Warning", Warning);
	BaloonNotificationManager::showBaloonNotification("This test should NOT succeed as Note", Note);

	BaloonNotificationManager::waitForBaloonsToFinish();
	BaloonNotificationManager::cleanUp();

	return 0;
}