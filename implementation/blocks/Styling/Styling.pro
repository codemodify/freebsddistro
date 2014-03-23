
# Set the configuration for these sources
##########################################################################
TEMPLATE     = lib
TARGET       = Styling
CONFIG      += qt shared
QT           = core gui


# include common config stuff
##########################################################################
include( ../../_builder_/common-options.pro )


# Settle what sources to use
##########################################################################
HEADERS += Styling.h
SOURCES += Styling.cpp

HEADERS += SystemTrayIcon/SystemTrayIcon.h
SOURCES += SystemTrayIcon/SystemTrayIcon.cpp
