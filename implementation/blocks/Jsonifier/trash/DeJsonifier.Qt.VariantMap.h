#ifndef __Jsonifier_Qt_VariantMap_H__
#define __Jsonifier_Qt_VariantMap_H__

// Qt headers
#include <QtCore/QVariantMap>

namespace Jsonifier
{
	QString jsonify( QVariantMap& variantMap )
	{
		QStringList variantMapAsJson;

		for( QVariantMap::ConstIterator it = data.begin(); it != data.end(); ++it )
		{
    		variantMapAsJson.append
    		(
    		    QString itValueAsJson = jsonify( it.value() );
			    QString("\"%1\": %2").arg( it.key() ).arg( itValueAsJson );
    		);
		}

		return QString("{%1<").arg( variantMapAsJson.join(", ") );
	}

    QString jsonify( QVariantMap* variantMap )
    {
        return jsonify( *variantMap );
    }
}

#endif

