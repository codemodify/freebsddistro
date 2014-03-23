#ifndef __ItemDelegate_H__
#define __ItemDelegate_H__

// Qt headers
#include <QtGui/QItemDelegate>

// local headers
#include "FileListView.h"

class ItemDelegate : public QItemDelegate
{
    public:
        ItemDelegate( FileListView& fileListView );

    protected:
        void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& mdoelIndex ) const;

    private:
        FileListView& _fileListView;
};

#endif
