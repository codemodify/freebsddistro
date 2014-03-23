
# Set the configuration for these sources
##########################################################################
TEMPLATE     = lib
TARGET       = CustomDockWidget
CONFIG      += qt thread shared
QT           = core gui


# include common config stuff
##########################################################################
include( ../../_builder_/common-options.pro )


# Settle what sources to use
##########################################################################
HEADERS += CustomDockWidget.h
SOURCES += CustomDockWidget.cpp

HEADERS += CustomDockWidget.TitleBar.h
SOURCES += CustomDockWidget.TitleBar.cpp

