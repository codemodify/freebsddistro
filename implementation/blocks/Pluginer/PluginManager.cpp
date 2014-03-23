
// local headers
#include "PluginManager.h"


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
//      Helpers
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
IPlugin* iplugin_cast( QPluginLoader* pluginLoader )
{
    QObject* pluginInstance = pluginLoader->instance();

    IPlugin* pluginInterface = qobject_cast< IPlugin* >( pluginInstance );

    return pluginInterface;    
}




// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
//      Constructors
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
PluginManager::PluginManager()
{
    _initOnPluginLoad = false;
}

PluginManager::~PluginManager()
{
    unLoadAllPlugins();
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
//      loads
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
IPlugin* PluginManager::loadPlugin( QString pluginPath, QString& errorString )
{
    QPluginLoader* pluginLoader = new QPluginLoader( pluginPath );

    // load the library
    if( false == pluginLoader->load() )
    {
        errorString = pluginLoader->errorString();

        delete pluginLoader;

        return 0;
    }

    // get the Qt object
    IPlugin* plugin = iplugin_cast( pluginLoader );
    if( 0 == plugin )
    {
        errorString = QString("%1: Plugin is not valid or doesn't derive from IPlugin").arg( pluginPath );

        pluginLoader->unload();

        delete pluginLoader;

        return 0;
    }

    // init the plugin
    if
    (
        ( true == _initOnPluginLoad )
        &&
        ( false == plugin->doInit() )
    )
    {
        errorString = QString("%1: Plugin Init() failed.").arg( pluginPath );

        pluginLoader->unload();

        delete pluginLoader;

        return 0;        
    }

    _loadedPlugins.insert( pluginPath, pluginLoader );

    return plugin;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
//      unloads
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void PluginManager::unLoadPlugin( QString pluginPath )
{
    QPluginLoader* pluginLoader = 0;

    if( _loadedPlugins.contains(pluginPath) )
        pluginLoader = _loadedPlugins[ pluginPath ];

    if( 0 == pluginLoader )
        return;

    // uninit the plugin
    IPlugin* plugin = iplugin_cast( pluginLoader );
             plugin->unInit();

    pluginLoader->unload();
    delete pluginLoader;

    _loadedPlugins.remove( pluginPath );
}

void PluginManager::unLoadAllPlugins()
{
    QPluginLoader* pluginLoader = 0;

    foreach( QString pluginPath, _loadedPlugins.keys() )
    {
        pluginLoader = _loadedPlugins[ pluginPath ];

        IPlugin* plugin = iplugin_cast( pluginLoader );
                 plugin->unInit();

        pluginLoader->unload();
        delete pluginLoader;
    }

    _loadedPlugins.clear();
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
//      getters
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void PluginManager::getLoadedPlugins( PluginList& pluginList )
{
    foreach( QPluginLoader* pluginLoader, _loadedPlugins.values() )
    {
        IPlugin* plugin = iplugin_cast( pluginLoader );

        pluginList.append( plugin );
    }
}

void PluginManager::getLoadedPlugins( PluginList& pluginList, QString subclassedClass )
{
    foreach( QPluginLoader* pluginLoader, _loadedPlugins.values() )
    {
        IPlugin* plugin = iplugin_cast( pluginLoader );

        if
        (
            (0 != plugin)
//            &&
//            (subclassedClass == plugin->getPluginInfo().rttiInheritedInterface)
        )
        {
            pluginList.append( plugin );
        }
    }
}

void PluginManager::getLoadedPlugins( QStringList& pluginList )
{
    foreach( QString path, _loadedPlugins.keys() )
    {
        pluginList.append( path );
    }
}

void PluginManager::getLoadedPlugins( PluginListWithPaths& pluginList )
{
    LoadedPlugins::const_iterator it    = _loadedPlugins.begin();
    LoadedPlugins::const_iterator end   = _loadedPlugins.end();

    while( it != end )
    {
        QString         path        = it.key();
        QPluginLoader*  pluginLoader= it.value();

        IPlugin* plugin = iplugin_cast( pluginLoader );

        pluginList.insert( path, plugin );

        it++;
    }
}

void PluginManager::getPluginByFileName( QString pluginFileName, IPlugin*& plugin )
{
    plugin = 0;

    if( _loadedPlugins.contains(pluginFileName) )
    {
        QPluginLoader* pluginLoader = _loadedPlugins[ pluginFileName ];

        plugin = iplugin_cast( pluginLoader );
    }
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
//      setters
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void PluginManager::setNoInitOnPluginLoad( bool initOnPluginLoad )
{
    _initOnPluginLoad = initOnPluginLoad;
}

