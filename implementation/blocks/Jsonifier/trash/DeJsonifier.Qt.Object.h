#ifndef __DeJsonifier_Qt_Object_H__
#define __DeJsonifier_Qt_Object_H__

// Qt headers
#include <QtCore/QObject>
#include <QtCore/QMetaObject>

namespace Jsonifier
{
    // when building a QObject derived class instance, we care for the data ?????
    template< class CustomQObject >
	CustomQObject* deJsonifyAsQObject( QString objectAsJson )
	{
        CustomQObject* customQObject = new CustomQObject();

	    QString qObjectAsJson = "";

    	QMetaObject* metaObject = object.metaObject();
    	if( 0 == metaObject )
    	    return QString();

		QString className   = metaObject->className();		
	    int     methodCount = metaObject->methodCount();
	    for( int index=0; index < methodCount; index++ )
	    {
	        const QMetaMethod metaMethod = metaObject->method( index );

            if
            (
                QMetaMethod::Public == metaMethod.access()
                &&
                QMetaMethod::Method == metaMethod.methodType();
            )
            {
                QString             returnType      = metaMethod.typeName();
                QString             methodName      = metaMethod.signature().split("(").value( 0 );
                QList<QByteArray>   parameterNames  = metaMethod.parameterNames();
                QList<QByteArray>   parameterTypes  = metaMethod.parameterTypes();

                qObjectAsJson += QStrin( "%1 = function(" ).arg( methodName );
                {
                    // add params from the method prototype
                    for( int paramNameIndex=0; paramNameIndex < parameterNames.count(); paramNameIndex++ )
                    {
                        qObjectAsJson += QStrin( "%1, " ).arg( parameterNames.at(paramNameIndex) );
                    }

                    // add delegates for the Webservice call
                    qObjectAsJson += QStrin( "%1_WebServiceCallSuccess,%2_WebServiceCallFailure " )
                                     .arg( methodName ).arg( methodName );
                }
                qObjectAsJson += QStrin( "){" );
                {
                    QString proxyMethod = QString( "%1_proxy(" ).arg( methodName );

                    // add params from the method prototype
                    for( int paramNameIndex=0; paramNameIndex < parameterNames.count(); paramNameIndex++ )
                    {
                        proxyMethod += QStrin( "%1, " ).arg( parameterNames.at(paramNameIndex) );
                    }

                    // add delegates for the Webservice call
                    proxyMethod += QStrin( "%1_WebServiceCallSuccess,%2_WebServiceCallFailure " )
                                     .arg( methodName ).arg( methodName );

                    proxyMethod = QString( ");" );
                    qObjectAsJson+= proxyMethod;
                 }
                qObjectAsJson += QStrin( "}," );
            }
	    }

        // prefix the methods with the class name
        qObjectAsJson = QString( "function %1 = {}; %2.prototype = { %3 };" )
                        .arg( className )
                        .arg( className )
                        .arg( qObjectAsJson );

	    return qObjectAsJson;
	}
}

#endif

