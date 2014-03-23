
// local headers
#include "DeJsonifier.h"

namespace Jsonifier
{
    void deJsonifyAsVariantMap( QString& variantMapAsJson, QVariantMap& variantMap )
	{
        variantMapAsJson.remove( 0, 1 );                            // remove the '{'
        variantMapAsJson.remove( variantMapAsJson.length()-1, 1 );  // remove the '}'

		QStringList variantElements = variantMapAsJson.split( "," );

        for( int index=0; index < variantElements.length(); index++ )
		{
            QString key   = variantElements.at( index ).split(":").value( 0 );
            QString value = variantElements.at( index ).split(":").value( 1 );

            variantMap.insert( deJsonifyAsString(key), deJsonifyAsVariant(value) );
		}
	}

    QVariantMap deJsonifyAsVariantMap( QString variantMapAsJson )
    {
        QVariantMap variantMap;

        deJsonifyAsVariantMap( variantMapAsJson, variantMap );

        return variantMap;
    }
}
