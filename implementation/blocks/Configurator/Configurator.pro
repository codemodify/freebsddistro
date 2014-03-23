
# Set the configuration for these sources
##########################################################################
TEMPLATE     = lib
TARGET       = Configurator
CONFIG      += qt thread shared
QT           = core gui xml webkit


# include common config stuff
##########################################################################
include( ../../_builder_/common-options.pro )


# Settle what sources to use
##########################################################################
HEADERS += Configurator.h
SOURCES += Configurator.cpp

HEADERS += Helper.h
