
# Set the configuration for these sources
##########################################################################
TEMPLATE     = lib
TARGET       = FileSearch
CONFIG      += qt thread shared
QT           = core


# include common config stuff
##########################################################################
include( ../../_builder_/common-options.pro )


# Settle what sources to use
##########################################################################
HEADERS += FileSearch.h
SOURCES += FileSearch.cpp

HEADERS += FileSearchModel/FileSearchModel.h
SOURCES += FileSearchModel/FileSearchModel.cpp
