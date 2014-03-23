
#include "cd-dvd-authoring/cd-dvd-authoring.h"

int main(int argc, char* argv[])
{
	// creating the qt-application object
	QApplication qapplication(argc, argv);
	qapplication.setQuitOnLastWindowClosed(true);
	
	// creating filemanager object, and showing it
	CDDVDAuthoring cddvdAuthoring;
	cddvdAuthoring.initEnvironement();
	cddvdAuthoring.show();
	
	// running the application
	return qapplication.exec();
}
