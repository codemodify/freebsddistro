#ifndef __View_H__
#define __View_H__

// Qt headers
#include <QtGui/QCloseEvent>

// blocks
#include <CustomDockWidget/CustomDockWidget.h>


class View : public CustomDockWidget
{
    Q_OBJECT

    public:
        View();
        virtual ~View();
  
};

#endif
