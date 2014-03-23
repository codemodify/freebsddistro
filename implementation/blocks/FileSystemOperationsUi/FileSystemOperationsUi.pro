
# Set the configuration for these sources
##########################################################################
TEMPLATE     = lib
TARGET       = FileSystemOperationsUi
CONFIG      += qt thread shared
QT           = core gui


# include common config stuff
##########################################################################
include( ../../_builder_/common-options.pro )


# Settle what sources to use
##########################################################################
HEADERS += FileSystemOperationsUi.h
SOURCES += FileSystemOperationsUi.cpp
