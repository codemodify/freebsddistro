
// Qt headers
#include "DeJsonifier.h"

namespace Jsonifier
{
    void deJsonifyAsVariantList( QString& variantListAsJson, QVariantList& variantList )
	{
        variantListAsJson.remove( 0, 1 );                            // remove the '['
        variantListAsJson.remove( variantListAsJson.length()-1, 1 );  // remove the ']'

		QStringList variantElements = variantListAsJson.split( "," );


        for( int index=0; index < variantElements.length(); index++ )
		{
            variantList.append( deJsonifyAsVariant(variantElements.at(index)) );
		}
	}

	QVariantList deJsonifyAsVariantList( QString variantListAsJson )
    {
        QVariantList variantList;

        deJsonifyAsVariantList( variantListAsJson, variantList );

        return variantList;
    }
}

