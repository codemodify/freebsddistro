
# External headers / libraries
##########################################################################
EXTERNAL_DEPENDECY_NAME__0001 = InternetServerPlatformLibrary
EXTERNAL_DEPENDECY_LIBS__0001 = -l$$EXTERNAL_DEPENDECY_NAME__0001


# Set the configuration for these sources
##########################################################################
TEMPLATE     = lib
TARGET       = WebServer
CONFIG      += qt thread shared plugin
QT           = core xml
LIBS        += $$EXTERNAL_DEPENDECY_LIBS__0001


# include common config stuff
##########################################################################
include( ../../_builder_/common-options.pro )


# Settle what sources to use
##########################################################################
HEADERS += WebServer.h
SOURCES += WebServer.cpp

HEADERS += HttpCodes.h
HEADERS += Configuration.h
