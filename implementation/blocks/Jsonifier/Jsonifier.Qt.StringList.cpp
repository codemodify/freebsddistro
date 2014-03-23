
// local headers
#include "Jsonifier.h"

namespace Jsonifier
{
	QString jsonify( QStringList stringList )
	{
		QStringList variantListAsJson;

        for( int index=0; index < stringList.length(); index++ )
		{
			variantListAsJson.append( QString("'%1'").arg(stringList.at(index)) );
		}
		
		return QString("[%1]").arg( variantListAsJson.join(",") );
	}
}
