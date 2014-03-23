#ifndef __TestPlugin3_H__
#define __TestPlugin3_H__

// Qt Headers
#include <QtGui/QWidget>

// blocks
#include <Pluginer/PluginImplementation.h>

// local headers
#include <!_tests_!/PluginerUi/Tester/TestInterface3.h>


class TestPlugin3 : public QObject, public AbstractPlugin, public TestInterface3
{
    Q_OBJECT
    Q_INTERFACES( IPlugin )
    Q_PLUGINABLE( TestPlugin3 )

    bool        doInit();
    void        unInit();
    QWidget*    getWidget();
};


#endif

