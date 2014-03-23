#ifndef __TestPlugin2_H__
#define __TestPlugin2_H__

// Qt Headers
#include <QtGui/QWidget>

// blocks
#include <Pluginer/PluginImplementation.h>

// local headers
#include <!_tests_!/PluginerUi/Tester/TestInterface2.h>


class TestPlugin2 : public QObject, public AbstractPlugin, public TestInterface2
{
    Q_OBJECT
    Q_INTERFACES( IPlugin )
    Q_PLUGINABLE( TestPlugin2 )

    bool        doInit();
    void        unInit();
    QWidget*    getWidget();
};


#endif

