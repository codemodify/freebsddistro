#ifndef __PluginManager_H__
#define __PluginManager_H__


// Qt headers
#include <QtCore/QList>
#include <QtCore/QStringList>
#include <QtCore/QMap>
#include <QtCore/QPluginLoader>
#include <QtCore/QMetaObject>
#include <QtCore/QObject>

// local headers
#include <Pluginer/IPlugin.h>



/***************************************************************************************************

    Pluginer contains:

    - PluginManager
        o Manage plugins and their state

    - IPlugin
        o interface that must be implemented by plugin providers,
          a plugin is a library that contains a Qt plugin ( declared via Q_EXPORT_PLUGIN2() )

    - AbstractPlugin
        o base and convenient class from where all the plugins must inherit

***************************************************************************************************/
class PluginManager
{
    // constructors
    public:
        PluginManager();
        virtual ~PluginManager();

    // loads
    public:
        IPlugin* loadPlugin( QString pluginPath, QString& errorString );

    // unloads
    public:
        void unLoadPlugin( QString pluginPath );
        void unLoadAllPlugins();

    // getters
    public:
        typedef QList< IPlugin*          > PluginList;
        typedef QMap < QString, IPlugin* > PluginListWithPaths;

        void getLoadedPlugins( PluginList&          pluginList                          );
        void getLoadedPlugins( PluginList&          pluginList, QString subclassedClass );
        void getLoadedPlugins( QStringList&         pluginList                          );
        void getLoadedPlugins( PluginListWithPaths& pluginList                          );

        void getPluginByFileName( QString pluginFileName, IPlugin*& plugin );


        template< class CustomInterface >
        friend void getLoadedPluginsFromPluginManager( PluginManager& pluginManager, QList< CustomInterface* >& pluginList );

    // setters
    public:
        void setNoInitOnPluginLoad( bool initOnPluginLoad=false );

    // internals
    private:

        typedef QMap< QString, QPluginLoader* > LoadedPlugins;
        LoadedPlugins _loadedPlugins;

        bool _initOnPluginLoad;

};


template< class CustomInterface >
void getLoadedPluginsFromPluginManager( PluginManager& pluginManager, QList< CustomInterface* >& pluginList )
{
    foreach( QPluginLoader* pluginLoader, pluginManager._loadedPlugins.values() )
    {
        QObject*            object          = pluginLoader->instance();
        CustomInterface*    customInterface = qobject_cast< CustomInterface* >( object );

        if( 0 != customInterface )
            pluginList.append( customInterface );
    }
}


#endif
