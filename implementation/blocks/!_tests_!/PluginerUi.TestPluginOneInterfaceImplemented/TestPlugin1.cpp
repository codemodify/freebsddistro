
#include "TestPlugin1.h"

bool TestPlugin1::doInit()
{
    _pluginInfo.name            = "TestPlugin1";
    _pluginInfo.description     = "TestPlugin1 Description";
    _pluginInfo.version         = "1.0";
    _pluginInfo.compatVersion   = "1.0";
    _pluginInfo.vendor          = "abcdefghi";
    _pluginInfo.copyright       = "abcdefghi (C) (TM) (R) and all the rest of smart letters";
    _pluginInfo.license         = "absolutely free";
    _pluginInfo.url             = "www.nikushor.eu";

    return true;
}

void TestPlugin1::unInit()
{}

QWidget* TestPlugin1::getWidget()
{
    QWidget* w = new QWidget();
             w->setWindowTitle( metaObject()->className() );
             w->setMinimumSize( 300, 300 );

    return w;
}

SetPluginImplementation( TestPlugin1 )

