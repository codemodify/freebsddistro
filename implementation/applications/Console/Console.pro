
# External library dependencies
##########################################################################
include( ../../blocks/IconProvider/IconProvider.pri                 )
include( ../../blocks/Styling/Styling.pri                           )
include( ../../blocks/Settings/Settings.pri                         )


# Set the configuration for these sources
#########################################################################
TEMPLATE     = app
TARGET       = Console
CONFIG      += qt thread
QT           = core gui


# Set output for all intermediate files to one place
##########################################################################
include( ../../_builder_/common-options.pro )


# Settle what sources to use
##########################################################################
SOURCES += main.cpp

SOURCES +=  Console.cpp
HEADERS +=  Console.h

SOURCES +=  EnhancedDockWidget/EnhancedDockWidget.cpp
HEADERS +=  EnhancedDockWidget/EnhancedDockWidget.h
