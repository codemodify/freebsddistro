
// Qt headers
#include <QtGui/QPainter>

// local headers
#include "ItemDelegate.h"


#define CurrentHighlighedLineColor  "#778899"
#define SelectedColor               "#ff0000"


ItemDelegate::ItemDelegate( FileListView& fileListView ) :
    QItemDelegate(),
    _fileListView( fileListView )
{}

void ItemDelegate::paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& modelIndex ) const
{
    QItemDelegate::paint( painter, option, modelIndex );

    painter->save();

    // if this is current line we need to paint a box around it
    if( _fileListView.currentIndex() == modelIndex )
    {
        QRect cursorRect = option.rect;
        cursorRect.setWidth( cursorRect.width() - 1 );
        cursorRect.setHeight( cursorRect.height() - 1 );

        painter->setPen( QColor(CurrentHighlighedLineColor) );
        painter->drawRect( cursorRect );
    }

    // if this item is selected then draw a square at right
    if( _fileListView.isItemSelected(modelIndex) )
    {
        QRect selectedRect = option.rect;

        int delta = 2;

        selectedRect.setX( selectedRect.width() - selectedRect.height() + delta );
        selectedRect.setWidth( selectedRect.width() - delta );

        selectedRect.setY( selectedRect.y() + delta );
        selectedRect.setHeight( selectedRect.height() - delta );

        painter->fillRect( selectedRect, QBrush(QColor(SelectedColor)) );
    }

    painter->restore();
}
