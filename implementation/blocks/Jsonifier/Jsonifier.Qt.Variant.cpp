
// Qt headers
#include "Jsonifier.h"

namespace Jsonifier
{
    QString jsonify( QVariantList  variantList );
    QString jsonify( QVariantMap   variantMap  );
    QString jsonify( QStringList   stringList  );

    QString jsonify( QVariant variant )
    {
		switch( variant.type() )
		{
			case QVariant::String: return QString("'%1'").arg
		    (
		        variant.toString().replace("\\", "\\\\").replace("\"", "\\\"")
	        );

			case QVariant::Map:         return jsonify( variant.toMap()           );
			case QVariant::List:        return jsonify( variant.toList()          );
			case QVariant::StringList:  return jsonify( variant.toStringList()    );
			case QVariant::Invalid:     return "null";

			default:
                return variant.toString();			
		}        
    }
}
