
// Qt headers
#include <QtCore/QCoreApplication>
#include <QtCore/QFile>

#include <QtCore/QObject>
#include <QtCore/QStringList>

// blocks
#include <Jsonifier/Jsonifier.h>

using namespace Jsonifier;

// local headers
#include "CustomQObject.h"

typedef void (*JsonTester)();

void runJsonifiersTests();
void runDeJsonifiersTests();


void runJsonifiersTests()
{
    QString applicationPath = QCoreApplication::applicationDirPath();

    QFile file( QCoreApplication::applicationFilePath() + ".jsonified" );
    if( file.open(QFile::WriteOnly) )
    {
        // C++
        int     value1 =  1000;
        double  value2 =  1000.01;
        bool    value3 =  false;
        bool    value4 =  1000.02;
        char*   value5 = "1000.03";
        
        // Qt
        QVariant    value6 = QVariant( QVariant::Invalid );
        QString     value7 = "QString";
        QStringList value8 = QStringList() << "String list item 1" << "String list item 2" << "String list item 3";

        QVariantList    value9 =  QVariantList() << true << "a string" << 1999 << false << 2000.02;  
        QVariantMap     value0;
                        value0.insert( "1", true     );
                        value0.insert( "2", "String" );
                        value0.insert( "3", 2000.01  );
                        value0.insert( "4", 0        );
                        //value0.insert( "4", QStringList() << "String list item 1" << "String list item 2" );    // this won't work

        // Qt objects
        QObject*        object1 = new QObject();
        CustomQObject*  object2 = new CustomQObject();

        // save all these as JSON
        file.write( QString("\n%1\n").arg(jsonify(value1)).toAscii() );
        file.write( QString("\n%1\n").arg(jsonify(value2)).toAscii() );
        file.write( QString("\n%1\n").arg(jsonify(value3)).toAscii() );
        file.write( QString("\n%1\n").arg(jsonify(value4)).toAscii() );
        file.write( QString("\n%1\n").arg(jsonify(value5)).toAscii() );
        file.write( QString("\n%1\n").arg(jsonify(value6)).toAscii() );
        file.write( QString("\n%1\n").arg(jsonify(value7)).toAscii() );
        file.write( QString("\n%1\n").arg(jsonify(value8)).toAscii() );
        file.write( QString("\n%1\n").arg(jsonify(value9)).toAscii() );
        file.write( QString("\n%1\n").arg(jsonify(value0)).toAscii() );

        file.write( QString("\n%1\n").arg(jsonify(object1)).toAscii() );
        file.write( QString("\n%1\n").arg(jsonify(object2)).toAscii() );

        file.close();
    }
}
