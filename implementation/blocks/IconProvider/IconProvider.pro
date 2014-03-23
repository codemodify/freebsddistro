
# Set the configuration for these sources
##########################################################################
TEMPLATE     = lib
TARGET       = IconProvider
CONFIG      += qt thread shared
QT           = core gui


# include common config stuff
##########################################################################
include( ../../_builder_/common-options.pro )


# Settle what sources to use
##########################################################################
HEADERS += IconProvider.h

HEADERS += FileSystemIconProvider/FileSystemIconProvider.h
SOURCES += FileSystemIconProvider/FileSystemIconProvider.cpp

HEADERS += ActionIconProvider/ActionIconProvider.h
SOURCES += ActionIconProvider/ActionIconProvider.cpp
