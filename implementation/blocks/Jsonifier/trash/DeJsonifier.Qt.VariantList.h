#ifndef __DeJsonifier_Qt_VariantList_H__
#define __DeJsonifier_Qt_VariantList_H__

// Qt headers
#include <QtCore/QVariant>

namespace Jsonifier
{
	QString jsonify( QVariantList& variantList )
	{
		QStringList variantListAsJson;

        for( int index=0; index < variantList.length() )
		{
		    QVariant variantObject = variantList.at( index );
		    
			variantListAsJson.append( jsonify(variantObject) );
		}
		
		return QString("[%1]").arg( variantListAsJson.join(", ") );
	}

	QString jsonify( QVariantList* variantList )
	{
	    return jsonify( *variantList );
	}
}

#endif

