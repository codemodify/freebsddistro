#ifndef __ActionIconProvider_H__
#define __ActionIconProvider_H__

// Qt headers
#include <QtCore/QMap>

// local headers
#include "../IconProvider.h"

class ActionIconProvider : public IconProvider
{
    public:
        ActionIconProvider();
        ~ActionIconProvider();

    public:
        QIcon& getIconForAction(        QString& action );
        QIcon& getIconForAction( const  QString& action );

    private:
        QMap< QString, QIcon > _actionIcons;

};

#endif
