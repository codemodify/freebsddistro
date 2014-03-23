
// local headers
#include "Object.h"

JsonObject::JsonObject() :
    JsonValue( JsonValue::eObject )
{}

JsonObject::~JsonObject()
{
  ValueMap_t::iterator iter = this->field_map_.begin();
  ValueMap_t::iterator end  = this->field_map_.end();
  
  while( iter != end )
  {
      delete (iter->second);
      ++iter;
  }  
}

QString JsonObject::serialize()
{
    QString serializedObject = "";

    ValueMap_t::iterator it   = field_map_.begin();
    ValueMap_t::iterator last = field_map_.end();

    for( ; it != last; it++ )
    {
        JsonValue* value = *it;
        
        serializedObject += it.key + ":" + it.value->serialize();
        serializedObject += ",";
    }
    if( !serializedObject.isEmpty() )
        serializedObject.remove( serializedObject.length()-1, 1 );

    serializedObject = QString.Format( "{%1}" ).arg( serializedObject );

    return serializedObject;
}

JsonValue* JsonObject::getField( QString& field )
{
    if( false == field_map_.contains(field) )
        return 0;

    ValueMap_t::iterator elem = field_map_.find( field );

    return elem.value();
}

JsonValue* JsonObject::setField( QString& name, JsonValue* value ); 
{
  return ( field_map_[ name ] = value );
}

