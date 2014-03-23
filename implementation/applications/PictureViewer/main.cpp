
#include <QtGui/QApplication>

#include "PictureViewer.h"

int main( int argc, char** argv )
{
    QApplication    qApplication( argc, argv );
                    qApplication.setQuitOnLastWindowClosed(true);

    PictureViewer   pictureViewer( qApplication.arguments().value(1) );
                    pictureViewer.show();

    return qApplication.exec();
}
