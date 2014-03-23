
// Qt headers
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QFile>

#include <QtCore/QObject>
#include <QtCore/QStringList>

// blocks
#include <Jsonifier/Jsonifier.h>
#include <Jsonifier/DeJsonifier.h>

using namespace Jsonifier;

// local headers
#include "CustomQObject.h"

typedef void (*JsonTester)();

void runJsonifiersTests();
void runDeJsonifiersTests();


int main( int argc, char** argv )
{
    QCoreApplication qApplication( argc, argv );

    QList< JsonTester > jsonTesters;
                        jsonTesters.append( runJsonifiersTests      );
                        jsonTesters.append( runDeJsonifiersTests    );

    foreach( JsonTester jsonTester, jsonTesters )
    {
        jsonTester();
    }

    return 0;
}
