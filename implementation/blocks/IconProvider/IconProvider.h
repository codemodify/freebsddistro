#ifndef __IconProvider_H__
#define __IconProvider_H__

// Qt headers
#include <QtCore/QCoreApplication>
#include <QtCore/QString>
#include <QtGui/QIcon>

#define appPath ( QCoreApplication::applicationDirPath() )


/****************************************************************************************
*
*   Interface class for icon providers.
*
****************************************************************************************/
class IconProvider
{
//    public:
//        virtual QIcon& getIconForItem(          QString& itemFullPath ) = 0;
//        virtual QIcon& getIconForItem( const    QString& itemFullPath ) = 0;

};

#endif
