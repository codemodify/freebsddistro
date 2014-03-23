
#include <QtGui/QApplication>
#include "WebBrowser/WebBrowser.h"

int main( int argc, char** argv )
{
    QApplication application( argc, argv );

    WebBrowser* webBrowser = new WebBrowser();
    webBrowser->show();

    return application.exec();
}
