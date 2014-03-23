
# Set the configuration for these sources
##########################################################################
TEMPLATE     = lib
TARGET       = InternetServerPlatformLibrary
CONFIG      += qt thread shared
QT           = core xml


# include common config stuff
##########################################################################
include( ../../_builder_/common-options.pro )


# Settle what sources to use
##########################################################################
HEADERS += Components/Components.h

SOURCES += Components/Bottles/Bottles.cpp

SOURCES += Components/Dom/Dom.cpp

SOURCES += Components/Logging/Logging.cpp
HEADERS += Components/Logging/Logger/Logger.h
SOURCES += Components/Logging/Logger/Logger.cpp

HEADERS += Modules/ISocketDaemon.h
