
// local headers
#include "Value.h"


JsonValue::JsonValue( JsonValue::JsonType type ) :
    _type( type )
{}

JsonValue::~JsonValue()
{}

JsonValue::JsonType JsonValue::getType()
{
    return _type;
}

