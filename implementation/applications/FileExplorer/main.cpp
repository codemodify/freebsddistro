#include "filemanager/filemanager.h"

int main(int argc, char* argv[])
{
	// creating the qt-application object
	QApplication qapplication(argc, argv);
	qapplication.setQuitOnLastWindowClosed(true);
	
	// creating filemanager object, and showing it
	Filemanager filemanager;
	filemanager.initEnvironement();
	filemanager.show();
	
	// running the application
	return qapplication.exec();
}
