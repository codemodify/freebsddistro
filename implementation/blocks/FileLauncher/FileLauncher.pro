
# Set the configuration for these sources
##########################################################################
TEMPLATE     = lib
TARGET       = FileLauncher
CONFIG      += qt shared
QT           = core gui


# include common config stuff
##########################################################################
include( ../../_builder_/common-options.pro )


# Settle what sources to use
##########################################################################
HEADERS += FileLauncher.h
SOURCES += FileLauncher.cpp
