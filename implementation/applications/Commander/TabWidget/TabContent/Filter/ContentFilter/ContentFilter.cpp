
// Qt headers
#include <QtCore/QFile>

// local headers
#include "ContentFilter.h"


ContentFilter::ContentFilter( FileListView& fileListView ) :
    Filter( fileListView )
{}

ContentFilter::~ContentFilter()
{}

void ContentFilter::addAdditionalWidgets( QHBoxLayout& /*hBoxLayout*/ )
{}

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

void ContentFilter::dataChanged( const QString& data )
{
    // reset the content to the original
    _fileListView.refreshContent();
    QString currentPath = _fileListView.getCurrentFolder();

    // take the items and apply the filter
    QStringList content;
    _fileListView.getContent( content );

    QStringList filteredItems;
    for( int i=0; i!= content.size(); ++i )
    {
        QString itemName = content[ i ];

        QString itemFullPath = QString( "%1/%2" ).arg( currentPath ).arg( itemName );

        if( fileContainsContent(itemFullPath,data) )
            filteredItems.append( itemFullPath.remove(currentPath+"/") );
    }

    _fileListView.setContent( filteredItems );
}
