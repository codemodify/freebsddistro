
// local headers
#include "Array.h"

JsonArray::JsonArray() :
    JsonValue( JsonValue::eArray )
{}

JsonArray::~JsonArray()
{
    qDeleteAll( this );

    clear();
}

QString JsonArray::serialize()
{    
    QString serializedArray = "";

    QList< JsonValue* >::const_iterator it   = begin();
    QList< JsonValue* >::const_iterator last = end();

    for( ; it != last; it++ )
    {
        JsonValue* value = *it;
        
        serializedArray += value->serialize();
        serializedArray += ",";
    }
    if( !serializedArray.isEmpty() )
        serializedArray.remove( serializedArray.length()-1, 1 );

    serializedArray = QString.Format( "[%1]" ).arg( serializedArray );

    return serializedArray;
}

