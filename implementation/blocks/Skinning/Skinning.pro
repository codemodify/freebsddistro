
# Set the configuration for these sources
##########################################################################
TEMPLATE     = lib
TARGET       = Skinning
CONFIG      += qt shared
QT           = core gui


# include common config stuff
##########################################################################
include( ../../_builder_/common-options.pro )


# Settle what sources to use
##########################################################################
#HEADERS += SkinStyle.h
#SOURCES += SkinStyle.cpp

#HEADERS += SkinInfoReader.h
#SOURCES += SkinInfoReader.cpp

HEADERS += EventFilter/EventFilter.h
SOURCES += EventFilter/EventFilter.cpp

HEADERS += EventFilter/MouseEventFilter.h
SOURCES += EventFilter/MouseEventFilter.cpp

HEADERS += EventFilter/EnabledChangeEventFilter.h
SOURCES += EventFilter/EnabledChangeEventFilter.cpp

HEADERS += EventFilter/PaintEventFilter.h
SOURCES += EventFilter/PaintEventFilter.cpp

HEADERS += SkinWidget.h
SOURCES += SkinWidget.cpp

HEADERS += SkinWindow.h
SOURCES += SkinWindow.cpp

#HEADERS += SkinButton.h
#SOURCES += SkinButton.cpp
