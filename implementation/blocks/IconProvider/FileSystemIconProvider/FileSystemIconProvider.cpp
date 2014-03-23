
// Qt headers
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QStringList>

// local headers
#include "FileSystemIconProvider.h"


#define FOLDER      "folder"
#define FILE        "file"
#define RESOURCES   "/resources/images/filetypes/"


FileSystemIconProvider::FileSystemIconProvider() :
    IconProvider()
{
    _fileTypeIcons.insert( FOLDER, QIcon(appPath + RESOURCES + "folder") );
    _fileTypeIcons.insert( FILE,   QIcon(appPath + RESOURCES + "file"  ) );

    QStringList sufixes = QDir( appPath + RESOURCES ).entryList();
    foreach( QString sufix, sufixes )
        _fileTypeIcons.insert( sufix, QIcon(appPath+RESOURCES+sufix) );
}

FileSystemIconProvider::~FileSystemIconProvider()
{
    _fileTypeIcons.clear();
}

QIcon& FileSystemIconProvider::getIconForFile( QString& itemFullPath )
{
    QString itemSuffix = FOLDER;

    if( !QFileInfo(itemFullPath).isDir() )
        itemSuffix = QFileInfo(itemFullPath).suffix();

    if( !_fileTypeIcons.contains(itemSuffix) )
        itemSuffix = FILE;

    return _fileTypeIcons[ itemSuffix ];
}

QIcon& FileSystemIconProvider::getIconForFile( const QString& itemFullPath )
{
    QString nonConstString = itemFullPath;
    return getIconForFile( nonConstString );
}

