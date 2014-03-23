
# External library dependencies
##########################################################################
include( ../../../blocks/FileSystemOperations/FileSystemOperations.pri                 )
include( ../../../blocks/FileSystemOperationsUi/FileSystemOperationsUi.pri             )

# Set the configuration for these sources
#########################################################################
TEMPLATE     = app
TARGET       = FileSystemOperationsUiTest
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
