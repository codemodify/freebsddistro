#ifndef __JsonSimpleType_h__
#define __JsonSimpleType_h__

// lcoal headers
#include "Value.h"

namespace jost
{
    template <typename T, jost::Value::TypeID TYPE_ID>
    class IntegralType;

    typedef jost::IntegralType<int, jost::Value::INTEGER>        Integer;
    typedef jost::IntegralType<float, jost::Value::REAL>         Real;
    typedef jost::IntegralType<bool, jost::Value::BOOLEAN>       Boolean;
    typedef jost::IntegralType<QString, jost::Value::STRING> String;

    typedef void* nil_t;
    typedef jost::IntegralType<nil_t, jost::Value::NIL>            Nil;
}

/****************************************************************************
*
*   Provides the abstraction for JSON types such as integer, real,
*   boolean, string.
*
****************************************************************************/
template < typename T, JsonValue::JsonType jsonType >
class SimpleType : public virtual JsonValue
{
    public:
        SimpleType( T& t ) :
            JsonValue( jsonType ),
            _value( t )
        {}

        ~SimpleType()
        {}

    public:
        T& value()
        {
            return _value;
        }

        QString serialize()
        {
            QString json_serialization =
                lexical_cast<QString>( this->_value );

            return json_serialization;
        }
        
    private:
        T _value;
};

template<> QString lexical_cast< QString, bool >( const bool& arg )
{
    QString ret_val;
    if (arg)
        ret_val = "true";
    else
        ret_val = "false"; 

    return ret_val;
}

template<> QString lexical_cast<QString, QString>( const QString& arg )
{
    return "\"" + arg + "\"";
}

template<> QString lexical_cast<QString, jost::nil_t>( const nil_t& arg ) 
{
    return "null";
}

#endif

