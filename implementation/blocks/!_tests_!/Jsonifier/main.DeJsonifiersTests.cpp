
// Qt headers
#include <QtCore/QCoreApplication>
#include <QtCore/QFile>

#include <QtCore/QObject>
#include <QtCore/QStringList>

// blocks
#include <Jsonifier/Jsonifier.h>
#include <Jsonifier/DeJsonifier.h>

using namespace Jsonifier;

// local headers
#include "CustomQObject.h"

void runDeJsonifiersTests()
{
    QString applicationPath = QCoreApplication::applicationDirPath();

    // run deJsonifiers
    QStringList values;

                // C++
                values.append( "1000"      ); // int
                values.append( "1000.01"   ); // double
                values.append( "false"     ); // bool
                values.append( "'1000.03'" ); // char*

                // QStringList
                values.append( "['String list item 1','String list item 2','String list item 3']" ); 
    
                // QVariantList
                values.append( "[true,'a string',1999,false,2000.02]" );
    
                // QVariantMap
                values.append( "{'1':true,'2':'String','3':2000.01,'4':0}" );
                // values.append( "{'1':true,'2':'String','3':2000.01,'4':['String list item 1','String list item 2']}" );     // this won't work

                // QObject
                values.append( "QObject.prototype={objectName=''};" );

                // CustomQObject
                values.append( "CustomQObject.prototype={Priority=2,User={'1':true,'2':'String','3':2000.01,'4':0]}};" );

    QFile fileDeJsonified( QCoreApplication::applicationFilePath() + ".deJsonified" );
    if( fileDeJsonified.open(QFile::WriteOnly) )
    {
        fileDeJsonified.write( QString("\n%1\n").arg(jsonify(    deJsonifyAsInt         (values[0])      )).toAscii() );
        fileDeJsonified.write( QString("\n%1\n").arg(jsonify(    deJsonifyAsDouble      (values[1])      )).toAscii() );
        fileDeJsonified.write( QString("\n%1\n").arg(jsonify(    deJsonifyAsBool        (values[2])      )).toAscii() );
        fileDeJsonified.write( QString("\n%1\n").arg(jsonify(    deJsonifyAsString      (values[3])      )).toAscii() );
        fileDeJsonified.write( QString("\n%1\n").arg(jsonify(    deJsonifyAsStringList  (values[4])      )).toAscii() );
        fileDeJsonified.write( QString("\n%1\n").arg(jsonify(    deJsonifyAsVariantList (values[5])      )).toAscii() );
        fileDeJsonified.write( QString("\n%1\n").arg(jsonify(    deJsonifyAsVariantMap  (values[6])      )).toAscii() );
        fileDeJsonified.write( QString("\n%1\n").arg(jsonify(    deJsonifyAsObject      (values[7])      )).toAscii() );
        fileDeJsonified.write( QString("\n%1\n").arg(jsonify(    deJsonifyAsObject      (values[8])      )).toAscii() );

        fileDeJsonified.close();
    }
}

