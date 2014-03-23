
// local headers
#include "TextEditor.h"

int main( int argc, char** argv )
{
    QApplication qApplication( argc, argv );
                 qApplication.setQuitOnLastWindowClosed( true );

    TextEditor  textEditor;
                textEditor.show();

    // open the the specified command line files if any
    if( qApplication.arguments().size() > 1 )
        for( int index=1; index < qApplication.arguments().size(); index++ )
            textEditor.newTab( qApplication.arguments().value(index) );

    return qApplication.exec();
}

