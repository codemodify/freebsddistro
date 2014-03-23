
#include <QtCore/QCoreApplication>

#include "coreinstaller/coreinstaller.h"

int main( int argc, char** argv )
{
	QCoreApplication coreApplication( argc, argv );
	
	CoreInstaller coreInstaller;
	coreInstaller.setInstallFile( argv[1] );
	
	coreApplication.exec();
}
