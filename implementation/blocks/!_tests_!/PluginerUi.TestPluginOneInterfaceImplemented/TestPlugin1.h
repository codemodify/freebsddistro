#ifndef __TestPlugin1_H__
#define __TestPlugin1_H__

// Qt Headers
#include <QtGui/QWidget>

// blocks
#include <Pluginer/PluginImplementation.h>

// local headers
#include <!_tests_!/PluginerUi/Tester/TestInterface1.h>


class TestPlugin1 : public QObject, public AbstractPlugin, public TestInterface1
{
    Q_OBJECT
    Q_INTERFACES( IPlugin )
    Q_PLUGINABLE( TestPlugin1 )

    bool        doInit();
    void        unInit();
    QWidget*    getWidget();
};


#endif

