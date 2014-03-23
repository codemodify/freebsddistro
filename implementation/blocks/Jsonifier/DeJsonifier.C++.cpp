
// local headers
#include "DeJsonifier.h"

namespace Jsonifier
{
    int deJsonifyAsInt( QString value ) 
    {
        return value.toInt();
    }

    double deJsonifyAsDouble( QString value ) 
    {
        return value.toDouble();
    }

    bool deJsonifyAsBool( QString value )
    {
        return
        (
            value == "true"  ? true : 
            (
                value == "1" ? true : false
            )
        );
    }

    QString deJsonifyAsString( QString value )
    {
        value.replace( "'", "" );

        return value;
    }
}
