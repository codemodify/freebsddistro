#ifndef __JsonArray_h__
#define __JsonArray_h__

// Qt headers
#include <QtCore/QList>

// local headers
#include "Value.h"


/****************************************************************************
*
*   Provides the abstraction for JSON arrays.
*
****************************************************************************/ 
class JsonArray : public JsonValue, public QList< JsonValue* >
{
    public:
        JsonArray();
        virtual ~JsonArray();
        virtual QString serialize();
    
};    

#endif

