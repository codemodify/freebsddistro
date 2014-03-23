
# Set the configuration for these sources
##########################################################################
TEMPLATE     = lib
TARGET       = PluginerUi
CONFIG      += qt thread shared
QT           = core gui


# include common config stuff
##########################################################################
include( ../../_builder_/common-options.pro )


# Settle what sources to use
##########################################################################
HEADERS += PluginerUi.h
SOURCES += PluginerUi.cpp

HEADERS += PluginDetails.h
SOURCES += PluginDetails.cpp
