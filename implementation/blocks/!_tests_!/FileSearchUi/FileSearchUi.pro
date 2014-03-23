
# External library dependencies
##########################################################################
include( ../../../blocks/FileSearch/FileSearch.pri                 )
include( ../../../blocks/FileSearchUi/FileSearchUi.pri             )
include( ../../../blocks/IconProvider/IconProvider.pri             )


# Set the configuration for these sources
#########################################################################
TEMPLATE     = app
TARGET       = FileSearchUiTest
CONFIG      += qt thread
QT           = core gui


# include common config stuff
##########################################################################
include( ../../../_builder_/common-options.pro )


# Settle what sources to use
##########################################################################
SOURCES += main.cpp

SOURCES += Tester/Tester.cpp
HEADERS += Tester/Tester.h
