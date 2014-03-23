#ifndef __DeJsonifier_Qt_Variant_H__
#define __DeJsonifier_Qt_Variant_H__

// Qt headers
#include <QtCore/QVariant>

namespace Jsonifier
{
    QVariant deJsonifyAsQVariant( QString variantObjectAsJson )
    {
		switch( value.type() )
		{
			case QVariant::String: return QString("\"%1\"").arg
		    (
		        value.toString().replace("\\", "\\\\").replace("\"", "\\\"")
	        );

			case QVariant::Map:         return jsonify( value.toMap()  );
			case QVariant::List:        return jsonify( value.toList() );
			case QVariant::StringList:  return parseStringList(value.toStringList());
			case QVariant::Invalid:     return "null";

			default:
                return value.toString();			
		}        
    }

    QString jsonify( QVariant* variantObject )
    {
        return jsonify( *variantObject );
    }
}

#endif

