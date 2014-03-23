
# Set the configuration for these sources
#########################################################################
TEMPLATE     = lib
TARGET       = PluginerUi.TestPluginTwoInterfacesImplemented
CONFIG      += qt thread
QT           = core gui


# include common config stuff
##########################################################################
include( ../../../_builder_/common-options.pro )


# Settle what sources to use
##########################################################################
HEADERS += TestPlugin2.h
SOURCES += TestPlugin2.cpp

HEADERS += TestPlugin3.h
SOURCES += TestPlugin3.cpp

