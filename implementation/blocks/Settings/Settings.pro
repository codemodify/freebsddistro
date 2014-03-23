
# Set the configuration for these sources
##########################################################################
TEMPLATE     = lib
TARGET       = Settings
CONFIG      += qt shared
QT           = core xml


# include common config stuff
##########################################################################
include( ../../_builder_/common-options.pro )


# Settle what sources to use
##########################################################################
HEADERS += Settings.h
SOURCES += Settings.cpp
