#ifndef __ContentFilter_H__
#define __ContentFilter_H__

// local headers
#include "../Filter.h"


class ContentFilter : public Filter
{
    Q_OBJECT

    public:
        ContentFilter( FileListView& fileListView );
        ~ContentFilter();

    protected slots:
        virtual void dataChanged( const QString& data );

    protected:
        virtual void addAdditionalWidgets( QHBoxLayout& hBoxLayout );

};

#endif
