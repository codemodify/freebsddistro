
// local headers
#include "Jsonifier.h"

namespace Jsonifier
{
	QString jsonify( QVariantMap variantMap )
	{
		QStringList variantMapAsJson;

		for( QVariantMap::ConstIterator it = variantMap.begin(); it != variantMap.end(); ++it )
		{
            QString itValueAsJson = jsonify( it.value() );

    		variantMapAsJson.append
    		(
			    QString("'%1':%2").arg( it.key() ).arg( itValueAsJson )
    		);
		}

		return QString("{%1}").arg( variantMapAsJson.join(",") );
	}
}
