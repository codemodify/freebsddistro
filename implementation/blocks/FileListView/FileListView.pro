
# Set the configuration for these sources
##########################################################################
TEMPLATE     = lib
TARGET       = FileListView
CONFIG      += qt thread shared
QT           = core gui


# include common config stuff
##########################################################################
include( ../../_builder_/common-options.pro )


# Settle what sources to use
##########################################################################
HEADERS += FileListView.h
SOURCES += FileListView.cpp

HEADERS += ItemDelegate/ItemDelegate.h
SOURCES += ItemDelegate/ItemDelegate.cpp

HEADERS += FolderContentModel/FolderContentModel.h
SOURCES += FolderContentModel/FolderContentModel.cpp
