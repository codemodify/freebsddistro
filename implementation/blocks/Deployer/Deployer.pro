
# Set the configuration for these sources
##########################################################################
TEMPLATE     = lib
TARGET       = Deployer
CONFIG      += qt thread shared
QT           = core xml


# include common config stuff
##########################################################################
include( ../../_builder_/common-options.pro )


# Settle what sources to use
##########################################################################
HEADERS += Deployer.h
SOURCES += Deployer.cpp

HEADERS += Helper.h
