
# External library dependencies
##########################################################################
include( ../InternetServerPlatformLibrary/InternetServerPlatformLibrary.pri )
include( ../../blocks/Pluginer/Pluginer.pri                                 )


# Set the configuration for these sources
##########################################################################
TEMPLATE     = app
TARGET       = InternetServerPlatform
CONFIG      += qt thread shared
QT           = core xml network


# include common config stuff
##########################################################################
include( ../../_builder_/common-options.pro )


# Settle what sources to use
##########################################################################
SOURCES += main.cpp

HEADERS += InternetServerPlatform.h
SOURCES += InternetServerPlatform.cpp
SOURCES += ISP.FileWatchers.cpp
SOURCES += ISP.RunTime.cpp
SOURCES += ISP.Settings.cpp
HEADERS += ISP.SettingsDefinitions.h

SOURCES += TcpServer/TcpServer.cpp
HEADERS += TcpServer/TcpServer.h

