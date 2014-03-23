
// Qt headers
#include <QtGui/QPushButton>
#include <QtGui/QHBoxLayout>

// local headers
#include "Tester.h"
#include "TestInterface1.h"
#include "TestInterface2.h"
#include "TestInterface3.h"


Tester::Tester() :
    QWidget()
{
    // init controls
    QPushButton* showPluginManagerUi
        = new QPushButton( "show PluginManager Ui" );

    QPushButton* showQWidgetDerivedObjectFromTheLoadedPlugin
        = new QPushButton( "show QWidget Derived Object From The Loaded Plugin" );

    // add controls to the visible area
    QVBoxLayout*    layout = new QVBoxLayout( this );
                    layout->addWidget( showPluginManagerUi );
                    layout->addWidget( showQWidgetDerivedObjectFromTheLoadedPlugin );

    // subscribe for the press events
    connect( showPluginManagerUi, SIGNAL(pressed()), 
        this, SLOT(slot_showPluginManagerUi()) );

    connect( showQWidgetDerivedObjectFromTheLoadedPlugin, SIGNAL(pressed()), 
        this, SLOT(slot_showQWidgetDerivedObjectFromTheLoadedPlugin()) );

     // create the test objects
    _pluginManager = new PluginManager();
    _pluginerUi    = new PluginerUi( *_pluginManager );
}

Tester::~Tester()
{
    delete _pluginerUi;
    delete _pluginManager;
}

void Tester::slot_showPluginManagerUi()
{
    _pluginerUi->show();
}

void Tester::slot_showQWidgetDerivedObjectFromTheLoadedPlugin()
{
    QList< TestInterface1* > pluginList1;
    QList< TestInterface2* > pluginList2;
    QList< TestInterface3* > pluginList3;

    getLoadedPluginsFromPluginManager< TestInterface1 >( *_pluginManager, pluginList1 );
    getLoadedPluginsFromPluginManager< TestInterface2 >( *_pluginManager, pluginList2 );
    getLoadedPluginsFromPluginManager< TestInterface3 >( *_pluginManager, pluginList3 );

    if( false == pluginList1.isEmpty() )
    {
        TestInterface1* testInterface = pluginList1.value( 0 );

        QWidget* w = testInterface->getWidget();
        w->show();
    }

    if( false == pluginList2.isEmpty() )
    {
        TestInterface2* testInterface = pluginList2.value( 0 );

        QWidget* w = testInterface->getWidget();
        w->show();
    }

    if( false == pluginList3.isEmpty() )
    {
        TestInterface3* testInterface = pluginList3.value( 0 );

        QWidget* w = testInterface->getWidget();
        w->show();
    }

}

