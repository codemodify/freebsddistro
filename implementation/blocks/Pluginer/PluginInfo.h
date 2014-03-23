#ifndef __PluginInfo_H__
#define __PluginInfo_H__

// Qt headers
#include <QtCore/QString>

struct PluginInfo
{
    // general info
    QString name;
    QString description;
    QString version;
    QString compatVersion;
    QString vendor;
    QString copyright;
    QString license;
    QString url;

    // runtime set info
    QString pluginPath;
};

#endif
