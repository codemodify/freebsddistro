
# Set the configuration for these sources
##########################################################################
TEMPLATE     = lib
TARGET       = Jsonifier
CONFIG      += qt thread shared
QT           = core


# include common config stuff
##########################################################################
include( ../../_builder_/common-options.pro )


# Settle what sources to use
##########################################################################
HEADERS += Jsonifier.h
SOURCES += Jsonifier.Qt.Object.cpp
SOURCES += Jsonifier.Qt.Variant.cpp
SOURCES += Jsonifier.Qt.VariantList.cpp
SOURCES += Jsonifier.Qt.VariantMap.cpp
SOURCES += Jsonifier.Qt.StringList.cpp

HEADERS += DeJsonifier.h
SOURCES += DeJsonifier.C++.cpp
SOURCES += DeJsonifier.Qt.Object.cpp
SOURCES += DeJsonifier.Qt.Variant.cpp
SOURCES += DeJsonifier.Qt.VariantList.cpp
SOURCES += DeJsonifier.Qt.VariantMap.cpp
SOURCES += DeJsonifier.Qt.StringList.cpp

