
// local headers
#include "AbstractPlugin.h"

AbstractPlugin::AbstractPlugin()
{}

AbstractPlugin::~AbstractPlugin()
{}

void AbstractPlugin::setPluginPath( QString pluginPath )
{
    _pluginInfo.pluginPath = pluginPath;
}

const PluginInfo& AbstractPlugin::getPluginInfo()
{
    return _pluginInfo;
}

