
# External library dependencies
##########################################################################
include( ../../../blocks/Jsonifier/Jsonifier.pri                 )

# Set the configuration for these sources
#########################################################################
TEMPLATE     = app
TARGET       = JsonifierTest
CONFIG      += qt thread
QT           = core gui


# include common config stuff
##########################################################################
include( ../../../_builder_/common-options.pro )


# Settle what sources to use
##########################################################################
SOURCES += main.cpp

HEADERS += CustomQObject.h
SOURCES += CustomQObject.cpp

SOURCES += main.JsonifiersTests.cpp
SOURCES += main.DeJsonifiersTests.cpp

