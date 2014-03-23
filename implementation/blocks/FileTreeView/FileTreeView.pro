
# Set the configuration for these sources
##########################################################################
TEMPLATE     = lib
TARGET       = FileTreeView
CONFIG      += qt thread shared
QT           = core gui


# include common config stuff
##########################################################################
include( ../../_builder_/common-options.pro )


# Settle what sources to use
##########################################################################
HEADERS += FileTreeView.h
SOURCES += FileTreeView.cpp

HEADERS += FileTreeModel/FileTreeModel.h
SOURCES += FileTreeModel/FileTreeModel.cpp

