#ifndef __JsonValue_h__
#define __JsonValue_h__

// Qt headers
#include <QtCore/QString>


/****************************************************************************
*
*   Provides the common abstraction for JSON types, and thus values.
*
*   It is assumed that "Value" objects contained in aggregated elements 
*   such as "Object" or "Array" are owned by their container object.
*
****************************************************************************/
class JsonValue 
{  
    public:
        enum JsonType
        {
            eValue,

            eArray, 
            eObject,

            eBoolean,
            eInteger,
            eNull,
            eReal,
            eString
        };

    public:
        virtual     ~JsonValue();
        virtual     QString serialize() = 0;
        JsonType    getType();

    protected:
        JsonValue( JsonType type );

    protected:
      JsonType _type;
};

template < typename CustomType > CustomType* json_cast( JsonValue* value ) 
{
    CustomType* customTypeValue = dynamic_cast< CustomType* >( value );

    return customTypeValue;
}

#endif

