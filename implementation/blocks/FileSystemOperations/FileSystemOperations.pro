
# Set the configuration for these sources
##########################################################################
TEMPLATE     = lib
TARGET       = FileSystemOperations
CONFIG      += qt thread shared
QT           = core


# include common config stuff
##########################################################################
include( ../../_builder_/common-options.pro )


# Settle what sources to use
##########################################################################
HEADERS += FileSystemOperations.h
SOURCES += FileSystemOperations.cpp

HEADERS += FileIoEngine/FileIoEngine.h
SOURCES += FileIoEngine/FileIoEngine.cpp

HEADERS += ThreadedOperation/ThreadedOperation.h
SOURCES += ThreadedOperation/ThreadedOperation.cpp

HEADERS += OperationsModel/OperationsModel.h
SOURCES += OperationsModel/OperationsModel.cpp
HEADERS += OperationsModel/Operation/Operation.h
SOURCES += OperationsModel/Operation/Operation.cpp
