
#include "TestPlugin3.h"

bool TestPlugin3::doInit()
{
    _pluginInfo.name            = "TestPlugin3";
    _pluginInfo.description     = "TestPlugin3 Description";
    _pluginInfo.version         = "1.0";
    _pluginInfo.compatVersion   = "1.0";
    _pluginInfo.vendor          = "abcdefghi";
    _pluginInfo.copyright       = "abcdefghi (C) (TM) (R) and all the rest of smart letters";
    _pluginInfo.license         = "absolutely free";
    _pluginInfo.url             = "www.nikushor.eu";

    return true;
}

void TestPlugin3::unInit()
{}

QWidget* TestPlugin3::getWidget()
{
    QWidget* w = new QWidget();
             w->setWindowTitle( metaObject()->className() );
             w->setMinimumSize( 300, 300 );

    return w;
}

SetPluginImplementation( TestPlugin3 )

