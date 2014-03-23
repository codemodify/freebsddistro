
#include "TestPlugin2.h"

bool TestPlugin2::doInit()
{
    _pluginInfo.name            = "TestPlugin2";
    _pluginInfo.description     = "TestPlugin2 Description";
    _pluginInfo.version         = "1.0";
    _pluginInfo.compatVersion   = "1.0";
    _pluginInfo.vendor          = "abcdefghi";
    _pluginInfo.copyright       = "abcdefghi (C) (TM) (R) and all the rest of smart letters";
    _pluginInfo.license         = "absolutely free";
    _pluginInfo.url             = "www.nikushor.eu";

    return true;
}

void TestPlugin2::unInit()
{}

QWidget* TestPlugin2::getWidget()
{
    QWidget* w = new QWidget();
             w->setWindowTitle( metaObject()->className() );
             w->setMinimumSize( 300, 300 );

    return w;
}

SetPluginImplementation( TestPlugin2 )

