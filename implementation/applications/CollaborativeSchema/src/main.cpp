
#include <QtGui/QApplication> 

#include "collaborativeschema/collaborativeschema.h"

int main( int argc, char** argv )
{
	QApplication application( argc, argv );
	
	CollaborativeSchema collaborativeSchema;
	collaborativeSchema.show();
		
	return application.exec();
}
