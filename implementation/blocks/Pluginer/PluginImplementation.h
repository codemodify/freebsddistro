#ifndef __PluginImplementation_H__
#define __PluginImplementation_H__

// Qt headers
#include <QtPlugin>
#include <QtCore/QObject>

// local headers
#include <Pluginer/AbstractPlugin.h>


#define Q_PLUGINABLE( ClassName )                       \
    public:                                             \
        ClassName():QObject(),AbstractPlugin(){}        \
        ~ClassName(){}


#define SetPluginImplementation( ClassName )            \
    Q_EXPORT_PLUGIN2( ClassName, ClassName )


#endif

