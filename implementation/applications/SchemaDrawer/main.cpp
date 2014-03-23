
// Qt headers
#include <QtGui/QApplication>

// local headers
#include "SchemaDrawer.h"

int main( int argc, char** argv )
{	
	QApplication	qApplication( argc, argv );

	SchemaDrawer	schemaDrawer;
				schemaDrawer.show();

    // open the the specified command line files if any
    if( qApplication.arguments().size() > 1 )
        for( int index=1; index < qApplication.arguments().size(); index++ )
            schemaDrawer.openSchemaByFilePath( qApplication.arguments().value(index) );

	return qApplication.exec();
}
