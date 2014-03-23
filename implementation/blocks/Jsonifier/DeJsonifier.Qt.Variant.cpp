
// Qt headers
#include "DeJsonifier.h"

namespace Jsonifier
{
    void deJsonifyAsVariant( QString& variantAsJson, QVariant& variant )
    {
        // string
        if( variantAsJson.startsWith("'") )
            variant.setValue( deJsonifyAsString(variantAsJson) );

        // bool
        else
        if( variantAsJson == "true" || variantAsJson == "false" )
            variant.setValue( deJsonifyAsBool(variantAsJson) );

        // double
        else
        if( variantAsJson.contains(".") )
            variant.setValue( deJsonifyAsDouble(variantAsJson) );

        // int
        else
            variant.setValue( deJsonifyAsInt(variantAsJson) );
    }

    QVariant deJsonifyAsVariant( QString variantAsJson )
    {
        QVariant variant;

        deJsonifyAsVariant( variantAsJson, variant );

        return variant;
    }
}
