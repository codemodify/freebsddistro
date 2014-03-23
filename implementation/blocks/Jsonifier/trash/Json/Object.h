#ifndef __JsonObject_h__
#define __JsonObject_h__

// Qt headers
#include <QtCore/QHash>

// local headers
#include "Value.h"


/****************************************************************************
*
*   Provides the abstraction for a JSON Object.
*
****************************************************************************/ 
class JsonObject : public virtual JsonValue
{
    public:
        typedef QHash< QString, JsonValue* >   ValueMap_t;
        typedef ValueMap_t::const_iterator const_iterator;
        typedef ValueMap_t::iterator       iterator;

    public:
        JsonObject();
        virtual ~JsonObject();

    public:
        JsonValue* getField( QString& field );
        JsonValue* setField( QString& name, Value* value ); 
        virtual QString serialize();

    private:
        ValueMap_t field_map_;
};

#endif

