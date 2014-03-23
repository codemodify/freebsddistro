
# External library dependencies
##########################################################################
include( ../../blocks/IconProvider/IconProvider.pri                 )
include( ../../blocks/FileListView/FileListView.pri                 )
include( ../../blocks/Styling/Styling.pri                           )
include( ../../blocks/Settings/Settings.pri                         )
include( ../../blocks/Configurator/Configurator.pri                 )
include( ../../blocks/ConfiguratorReader/ConfiguratorReader.pri     )
include( ../../blocks/FileLauncher/FileLauncher.pri                 )
include( ../../blocks/Skinning/Skinning.pri                         )


# Set the configuration for these sources
#########################################################################
TEMPLATE     = app
TARGET       = CommunicationCenter
CONFIG      += qt thread
QT           = core gui webkit


# Set output for all intermediate files to one place
##########################################################################
include( ../../_builder_/common-options.pro )


# Settle what sources to use
##########################################################################
SOURCES += main.cpp

SOURCES	+=	CommunicationCenter.cpp
HEADERS	+=	CommunicationCenter.h

SOURCES	+=	CommunicationCenterServerClient/CommunicationCenterServerClient.cpp
HEADERS	+=	CommunicationCenterServerClient/CommunicationCenterServerClient.h
