#ifndef __Jsonifier_H__
#define __Jsonifier_H__


/****************************************************************************

    Json-nifies

        C++: all types
        Qt : QObject derivatives that have Q_OBJECT declared
             +
            QVariant ==> QStringList, QVariantList, QVariantMap

*****************************************************************************/

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtCore/QVariantList>
#include <QtCore/QVariantMap>
#include <QtCore/QStringList>

namespace Jsonifier
{
    QString jsonify( QObject*   object  );
    QString jsonify( QVariant   variant );
}


#endif

