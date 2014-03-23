
// local headers
#include "CustomQObject.h"


CustomQObject::CustomQObject()
{
    _priority = 2;

    _user.insert( "1", true     );
    _user.insert( "2", "String" );
    _user.insert( "3", 2000.01  );
    _user.insert( "4", 0        );
}

CustomQObject::~CustomQObject(){}

void CustomQObject::testMethod1(){}

// C++ params
void CustomQObject::testMethod2 ( int           a ){}
void CustomQObject::testMethod3 ( double        a ){}
void CustomQObject::testMethod4 ( bool          a ){}
void CustomQObject::testMethod5 ( char*         a ){}

// Qt params
void CustomQObject::testMethod6 ( QVariant      a ){}
void CustomQObject::testMethod7 ( QString       a ){}
void CustomQObject::testMethod8 ( QStringList   a ){}

void CustomQObject::testMethod9 ( QVariantList  a ){}
void CustomQObject::testMethod10( QVariantMap   a ){}

void CustomQObject::testMethod11( QObject*      a ){}

void CustomQObject::setPriority ( int priority    )      { _priority = priority;}
int  CustomQObject::getPriority (                 ) const{ return _priority;    }

void        CustomQObject::setUser( QVariantMap user )      { _user = user; }
QVariantMap CustomQObject::getUser(                  ) const{ return _user; }

