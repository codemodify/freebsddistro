
// Qt headers
#include <QtCore/QFileInfo>
#include <QtCore/QDir>

// local headers
#include "FileSearch.h"


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- constructor area
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
FileSearch::FileSearch() :
    QThread()
{
    _fileSearchModel = new FileSearchModel();

    connect(    this,               SIGNAL  (itemAvailable(QString)),
                _fileSearchModel,   SLOT    (itemAvailable(QString))    );
}

FileSearch::~FileSearch()
{
    if( _fileSearchModel )
        delete _fileSearchModel;
}

QAbstractItemModel* FileSearch::getModel()
{
    return _fileSearchModel;
}

void FileSearch::findItems( QString targetFolder, QString nameFilter, QString contentFilter, bool folders )
{
    _targetFolder   = targetFolder;
    _nameFilter     = nameFilter;
    _contentFilter  = contentFilter;
    _folders        = folders;

    start();
}

void FileSearch::getItems( QStringList& items )
{
    items += _fileSearchModel->getItems();
}

void FileSearch::getFullContentForFolder( QStringList& content )
{
    // add this so we can use the algo below
    content.append( _targetFolder );

    for( int index=0; index<content.size(); ++index )
    {
        // getting the item
        QString itemPath = content.at( index );

        // item is a folder then scanning for its content
        if( !QFileInfo(itemPath).isDir() )
            continue;

        QStringList itemContent =
            QDir(itemPath).entryList( QDir::Hidden | QDir::System | QDir::CaseSensitive | QDir::NoDotAndDotDot | QDir::AllEntries );

        // here we fix the paths to be full paths
        for( int i=0; i!=itemContent.size(); ++i )
        {
            QString newItem = QString( "%1/%2" ).arg( itemPath ).arg( itemContent[i] );

            content.append( newItem );
        }

        itemContent.clear();
    }
}

bool fileContainsContent( QString filePath, QString contentToLookFor )
{
    if( contentToLookFor.isEmpty() )
        return true;

    QFile file( filePath );
    if( file.open(QFile::ReadOnly) )
    {
        QByteArray buffer;
        while( buffer=file.read(1048576), !buffer.isEmpty() )
        {
            if( buffer.contains(contentToLookFor.toAscii()) )
            {
                file.close();
                return true;
            }
        }

        file.close();
    }

    return false;
}

void FileSearch::run()
{
	_fileSearchModel->clear();

    QStringList targetFolderContent;
    getFullContentForFolder( targetFolderContent );

    // folders wanted
    if( true == _folders )
    {
        foreach( QString item, targetFolderContent )
        {
            if( QFileInfo(item).isDir() && item.contains(_nameFilter) )
                emit itemAvailable( item );
        }

        targetFolderContent.clear();
    }

    // files wanted
    else
    {
		// remove all folder
		for( int index=0; index < targetFolderContent.size(); )
		{
			QString item = targetFolderContent.value( index );

			if( QFileInfo(item).isDir() )
				targetFolderContent.removeAt( index );
			else
				index++;
		}

		// filter them out by name
		if( !_nameFilter.isEmpty() )
		{
			for( int index=0; index < targetFolderContent.size(); )
			{
				QString item = targetFolderContent.value( index );

				if( !QFileInfo(item).isDir() && item.contains(_nameFilter) )
					index++;
				else
					targetFolderContent.removeAt( index );
			}
		}

		// filter them out by content
		if( !_contentFilter.isEmpty() )
		{
			for( int index=0; index < targetFolderContent.size(); )
        		{
				QString item = targetFolderContent.value( index );

            		bool result = fileContainsContent( item, _contentFilter );
            		if( result )
                		index++;
            		else
                		targetFolderContent.removeAt( index );
        		}
		}
    }

    foreach( QString item, targetFolderContent )
        _fileSearchModel->itemAvailable( item );

    targetFolderContent.clear();
}
