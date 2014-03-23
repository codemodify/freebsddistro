
# Set the configuration for these sources
#########################################################################
TEMPLATE     = lib
TARGET       = PluginerUi.TestPluginOneInterfaceImplemented
CONFIG      += qt thread
QT           = core gui


# include common config stuff
##########################################################################
include( ../../../_builder_/common-options.pro )


# Settle what sources to use
##########################################################################
HEADERS += TestPlugin1.h
SOURCES += TestPlugin1.cpp

