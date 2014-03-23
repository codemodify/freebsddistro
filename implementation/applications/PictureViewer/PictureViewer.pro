
# External library dependencies
##########################################################################
include( ../../blocks/IconProvider/IconProvider.pri                 )
include( ../../blocks/Styling/Styling.pri                           )
include( ../../blocks/Settings/Settings.pri                         )
include( ../../blocks/FileEditor/FileEditor.pri                     )
include( ../../blocks/FileListView/FileListView.pri                 )


# Set the configuration for these sources
#########################################################################
TEMPLATE     = app
TARGET       = PictureViewer
CONFIG      += qt thread
QT           = core gui


# Set output for all intermediate files to one place
##########################################################################
include( ../../_builder_/common-options.pro )


# Settle what sources to use
##########################################################################
SOURCES += main.cpp

SOURCES +=  PictureViewer.cpp
HEADERS +=  PictureViewer.h
