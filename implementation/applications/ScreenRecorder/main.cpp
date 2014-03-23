
// Qt headers
#include <QtGui/QApplication>

// local headers
#include "ScreenRecorder.h"

int main( int argc, char** argv )
{
    QApplication qApplication( argc, argv );
                 qApplication.setQuitOnLastWindowClosed( true );

    ScreenRecorder  screenRecorder;
                    screenRecorder.show();

    return qApplication.exec();
}

