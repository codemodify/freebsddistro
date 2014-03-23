#ifndef __AbstractPlugin_H__
#define __AbstractPlugin_H__

// local headers
#include <Pluginer/IPlugin.h>


class AbstractPlugin : public IPlugin
{
    public:
        AbstractPlugin();
        virtual ~AbstractPlugin();

    public:
        virtual bool doInit() = 0;
        virtual void unInit() = 0;

    public:
        const PluginInfo&   getPluginInfo();
        void                setPluginPath( QString pluginPath );

    protected:
        PluginInfo  _pluginInfo;
 };

#endif

