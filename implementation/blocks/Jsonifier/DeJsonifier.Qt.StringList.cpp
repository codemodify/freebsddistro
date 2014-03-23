
// local headers
#include "DeJsonifier.h"

namespace Jsonifier
{
    void deJsonifyAsStringList( QString& stringListAsJson, QStringList& stringList )
	{
        stringListAsJson.replace( "[", "" );
        stringListAsJson.replace( "]", "" );
        stringListAsJson.replace( "'", "" );

        stringList = stringListAsJson.split( "," );
	}

	QStringList deJsonifyAsStringList( QString stringListAsJson )
    {
        QStringList stringList;

        deJsonifyAsStringList( stringListAsJson, stringList );

        return stringList;
    }
}
