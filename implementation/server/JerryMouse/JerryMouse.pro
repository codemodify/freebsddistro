
# External headers / libraries
##########################################################################
EXTERNAL_DEPENDECY_NAME__0001 = InternetServerPlatformLibrary
EXTERNAL_DEPENDECY_LIBS__0001 = -l$$EXTERNAL_DEPENDECY_NAME__0001


# Set the configuration for these sources
##########################################################################
TEMPLATE     = lib
TARGET       = JerryMouse
CONFIG      += qt thread shared plugin
QT           = core xml
LIBS        += $$EXTERNAL_DEPENDECY_LIBS__0001


# include common config stuff
##########################################################################
include( ../../_builder_/common-options.pro )


# Settle what sources to use
##########################################################################
HEADERS += JerryMouse.h
SOURCES += JerryMouse.cpp
