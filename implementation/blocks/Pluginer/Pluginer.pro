
# Set the configuration for these sources
##########################################################################
TEMPLATE     = lib
TARGET       = Pluginer
CONFIG      += qt thread shared
QT           = core


# include common config stuff
##########################################################################
include( ../../_builder_/common-options.pro )


# Settle what sources to use
##########################################################################
HEADERS += IPlugin.h

HEADERS += AbstractPlugin.h
SOURCES += AbstractPlugin.cpp

HEADERS += PluginImplementation.h
HEADERS += PluginDefinition.h

HEADERS += PluginManager.h
SOURCES += PluginManager.cpp

HEADERS += PluginInfo.h
