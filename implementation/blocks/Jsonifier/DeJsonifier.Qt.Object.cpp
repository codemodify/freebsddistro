
// local headers
#include "DeJsonifier.h"

namespace Jsonifier
{
    void deJsonifyAsObject( QString& objectAsJson, QObject*& object )
	{
        QString objectName = objectAsJson.mid( 0, objectAsJson.indexOf(".prototype=") );

//////////        object = 

//	    QString qObjectAsJson = "";
//
//    	const QMetaObject* metaObject = object.metaObject();
//    	if( 0 == metaObject )
//    	    return QString();
//
//		QString className       = metaObject->className     ();
//	    int     methodOffset    = metaObject->methodOffset  ();
//	    int     methodCount     = metaObject->methodCount   ();
//	    int     propertyOffset  = metaObject->propertyOffset();
//	    int     propertyCount   = metaObject->propertyCount ();
//
//        // look for methods
//	    for( int index=methodOffset; index < methodCount; index++ )
//	    {
//	        const QMetaMethod metaMethod = metaObject->method( index );
//
//            if
//            (
//                QMetaMethod::Public == metaMethod.access()
//                &&
//                QMetaMethod::Method == metaMethod.methodType()
//            )
//            {
//                QString             returnType      = metaMethod.typeName();
//                QString             methodName      = QString(metaMethod.signature()).split("(").value( 0 );
//                QList<QByteArray>   parameterNames  = metaMethod.parameterNames();
//                QList<QByteArray>   parameterTypes  = metaMethod.parameterTypes();
//
//                // build the JSON analog method
//                qObjectAsJson += QString( "%1=function(" ).arg( methodName );
//                {
//                    // add params from the method prototype
//                    for( int paramNameIndex=0; paramNameIndex < parameterNames.count(); paramNameIndex++ )
//                    {
//                        qObjectAsJson += QString( "%1," ).arg( parameterNames.at(paramNameIndex).data() );
//                    }
//
//                    // add delegates for the Webservice call
//                    qObjectAsJson += QString( "%1_WSCS,%2_WSCF" ).arg( methodName ).arg( methodName ); // Web ServiceC all Success - Web Service Call Failure
//                }
//
//                // add the proxy calls
//                qObjectAsJson += QString( "){" );
//                {
//                    QString proxyMethod = QString( "%1_proxy(" ).arg( methodName );
//                    
//                    // add params from the method prototype
//                    for( int paramNameIndex=0; paramNameIndex < parameterNames.count(); paramNameIndex++ )
//                    {
//                        proxyMethod += QString( "%1," ).arg( parameterNames.at(paramNameIndex).data() );
//                    }
//                    
//                    // add delegates for the Webservice call
//                    proxyMethod += QString( "%1_WSCS,%2_WSCF" ).arg( methodName ).arg( methodName );
//                    proxyMethod += QString( ");" );
//
//                    qObjectAsJson+= proxyMethod;
//                }
//
//                qObjectAsJson += QString( "}," );
//            }
//	    }
//
//        // look for members
//	    for( int index = propertyOffset; index < propertyCount; index++ )
//	    {
//            QMetaProperty property = metaObject->property( index );
//
//            QString propertyAsJson = QString( "%1=%2" )
//                                     .arg( property.name() )
//                                     .arg( jsonify(property.read(&object)) );
//
//            qObjectAsJson += QString( "%1," ).arg( propertyAsJson );
//        }
//
//        if( qObjectAsJson.endsWith(",") )
//            qObjectAsJson.remove( qObjectAsJson.lastIndexOf(","), 1 );
//
//        // prefix the methods with the class name
//        qObjectAsJson = QString( "function %1 = {}; %2.prototype = { %3 };" )
//                        .arg( className )
//                        .arg( className )
//                        .arg( qObjectAsJson );
//
//	    return qObjectAsJson;
	}

	QObject* deJsonifyAsObject( QString objectAsJson )
    {
        QObject* object = 0;

        deJsonifyAsObject( objectAsJson, object );

        return object;
    }
}
