
// Qt headers
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QStringList>

// local headers
#include "ActionIconProvider.h"


#define RESOURCES "/resources/images/actions/"


ActionIconProvider::ActionIconProvider() :
    IconProvider()
{}

ActionIconProvider::~ActionIconProvider()
{
    _actionIcons.clear();
}

QIcon& ActionIconProvider::getIconForAction( QString& action )
{
    // implementing caching for the loaded icons
    if( !_actionIcons.contains(action) )
        _actionIcons.insert( action, QIcon(appPath+RESOURCES+action) );

    return _actionIcons[ action ];
}

QIcon& ActionIconProvider::getIconForAction( const QString& itemFullPath )
{
    QString nonConstString = itemFullPath;
    return getIconForAction( nonConstString );
}

