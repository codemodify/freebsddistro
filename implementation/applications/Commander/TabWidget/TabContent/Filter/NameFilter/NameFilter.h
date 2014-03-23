#ifndef __NameFilter_H__
#define __NameFilter_H__

// local headers
#include "../Filter.h"


class NameFilter : public Filter
{
    Q_OBJECT

    public:
        NameFilter( FileListView& fileListView );
        ~NameFilter();

    protected slots:
        virtual void dataChanged( const QString& data );

    protected:
        virtual void addAdditionalWidgets( QHBoxLayout& hBoxLayout );

};

#endif
