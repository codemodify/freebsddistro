#include <QtGui/QApplication>
#include "dellirium/dellirium.h"

int main ( int argc, char** argv )
{
	QApplication qApplication( argc, argv );

	Dellirium dellirium;
	dellirium.show();

    return qApplication.exec();
}
