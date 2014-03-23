
# External library dependencies
##########################################################################
include( ../../../blocks/Configurator/Configurator.pri                 )
include( ../../../blocks/Pluginer/Pluginer.pri                         )
include( ../../../blocks/PluginerUi/PluginerUi.pri                     )


# Set the configuration for these sources
#########################################################################
TEMPLATE     = app
TARGET       = ConfiguratorTest
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
