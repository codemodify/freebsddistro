
// Qt headers
#include "Jsonifier.h"

namespace Jsonifier
{
	QString jsonify( QVariantList variantList )
	{
		QStringList variantListAsJson;

        for( int index=0; index < variantList.length(); index++ )
		{
		    QVariant variantObject = variantList.at( index );
		    
			variantListAsJson.append( jsonify(variantObject) );
		}
		
		return QString("[%1]").arg( variantListAsJson.join(",") );
	}
}

