#ifndef __CustomQObject_H__
#define __CustomQObject_H__

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariantList>
#include <QtCore/QVariantMap>


class CustomQObject : public QObject
{
    Q_OBJECT
    
    public:
        CustomQObject();
        ~CustomQObject();

    public:
        Q_INVOKABLE void testMethod1();

        // C++ params
        Q_INVOKABLE void testMethod2 ( int           a );
        Q_INVOKABLE void testMethod3 ( double        a );
        Q_INVOKABLE void testMethod4 ( bool          a );
        Q_INVOKABLE void testMethod5 ( char*         a );

        // Qt params
        Q_INVOKABLE void testMethod6 ( QVariant      a );
        Q_INVOKABLE void testMethod7 ( QString       a );
        Q_INVOKABLE void testMethod8 ( QStringList   a );

        Q_INVOKABLE void testMethod9 ( QVariantList  a );
        Q_INVOKABLE void testMethod10( QVariantMap   a );

        Q_INVOKABLE void testMethod11( QObject*      a );

    public:
        // export to JSON one single variable
        Q_PROPERTY( int Priority READ getPriority WRITE setPriority )
        int  _priority;
        void setPriority( int priority );
        int  getPriority(              ) const;

        // export to JSON a complex structure
        Q_PROPERTY( QVariantMap User READ getUser WRITE setUser )
        QVariantMap _user;
        void        setUser( QVariantMap user );
        QVariantMap getUser(                  ) const;
};

#endif
