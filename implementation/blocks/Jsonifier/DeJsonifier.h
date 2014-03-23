#ifndef __DeJsonifier_H__
#define __DeJsonifier_H__


/****************************************************************************

    De Json-nifies

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
    int             deJsonifyAsInt          ( QString intAsJson         );
    double          deJsonifyAsDouble       ( QString doubleAsJson      );
    bool            deJsonifyAsBool         ( QString boolAsJson        );
    QString         deJsonifyAsString       ( QString stringAsJson      );

    QObject*        deJsonifyAsObject       ( QString objectAsJson      );
    QVariant        deJsonifyAsVariant      ( QString objectAsVariant   );
    QVariantList    deJsonifyAsVariantList  ( QString variantListAsJson );
    QVariantMap     deJsonifyAsVariantMap   ( QString variantMapAsJson  );
    QStringList     deJsonifyAsStringList   ( QString stringListAsJson  );

    void deJsonifyAsObject      ( QString& objectAsJson     , QObject*    & object      );
    void deJsonifyAsVariant     ( QString& objectAsVariant  , QVariant    & variant     );
    void deJsonifyAsVariantList ( QString& variantListAsJson, QVariantList& variantList );
    void deJsonifyAsVariantMap  ( QString& variantMapAsJson , QVariantMap & variantMap  );
    void deJsonifyAsStringList  ( QString& stringListAsJson , QStringList & stringList  );
}

#endif

