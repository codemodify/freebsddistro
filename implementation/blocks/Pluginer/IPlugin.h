#ifndef __IPlugin_H__
#define __IPlugin_H__

// Qt headers
#include <QtPlugin>

// local headers
#include <Pluginer/PluginInfo.h>


class IPlugin
{
    public:
        virtual ~IPlugin(){}

        virtual bool doInit() = 0;
        virtual void unInit() = 0;
         
        virtual const PluginInfo& getPluginInfo()                      = 0;
        virtual void              setPluginPath( QString pluginPath )  = 0;
};

Q_DECLARE_INTERFACE( IPlugin, "IPlugin" )

#endif

