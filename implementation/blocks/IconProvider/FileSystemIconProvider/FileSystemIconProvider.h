#ifndef __FileSystemIconProvider_H__
#define __FileSystemIconProvider_H__

// Qt headers
#include <QtCore/QMap>

// local headers
#include "../IconProvider.h"

class FileSystemIconProvider : public IconProvider
{
    public:
        FileSystemIconProvider();
        ~FileSystemIconProvider();

    public:
        QIcon& getIconForFile(          QString& itemFullPath );
        QIcon& getIconForFile( const    QString& itemFullPath );

    private:
        QMap< QString, QIcon > _fileTypeIcons;

};

#endif
