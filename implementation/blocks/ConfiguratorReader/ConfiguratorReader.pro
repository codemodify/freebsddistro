
# Set the configuration for these sources
##########################################################################
TEMPLATE     = lib
TARGET       = ConfiguratorReader
CONFIG      += qt thread shared
QT           = core gui xml


# include common config stuff
##########################################################################
include( ../../_builder_/common-options.pro )


# Settle what sources to use
##########################################################################
HEADERS += ConfiguratorReader.h
SOURCES += ConfiguratorReader.cpp
