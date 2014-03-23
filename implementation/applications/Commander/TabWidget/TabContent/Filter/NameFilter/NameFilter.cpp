
// local headers
#include "NameFilter.h"


NameFilter::NameFilter( FileListView& fileListView ) :
    Filter( fileListView )
{}

NameFilter::~NameFilter()
{}

void NameFilter::addAdditionalWidgets( QHBoxLayout& /*hBoxLayout*/ )
{}

void NameFilter::dataChanged( const QString& data )
{
    QStringList content;

    // reset the content to the original
    _fileListView.refreshContent();

    // take the items and apply the filter
    _fileListView.getContent( content );

    QStringList filteredItems;
    for( int i=0; i!= content.size(); ++i )
    {
        QString itemName = content[ i ];

        if( itemName.contains(data) )
            filteredItems.append( itemName );
    }

    _fileListView.setContent( filteredItems );
}
