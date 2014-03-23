
# External library dependencies
##########################################################################
include( ../../blocks/IconProvider/IconProvider.pri                 )
include( ../../blocks/Styling/Styling.pri                           )
include( ../../blocks/Settings/Settings.pri                         )


# Set the configuration for these sources
#########################################################################
TEMPLATE     = app
TARGET       = SchemaDrawer
CONFIG      += qt thread
QT           = core gui


# Set output for all intermediate files to one place
##########################################################################
include( ../../_builder_/common-options.pro )


# Settle what sources to use
##########################################################################
SOURCES += main.cpp

SOURCES += SchemaDrawer.cpp
HEADERS += SchemaDrawer.h

SOURCES += DocumentDrawer/DocumentDrawer.cpp
HEADERS += DocumentDrawer/DocumentDrawer.h

SOURCES += DocumentDrawer/PageDrawer/PageDrawer.cpp
HEADERS += DocumentDrawer/PageDrawer/PageDrawer.h
HEADERS += DocumentDrawer/PageDrawer/PageFormat.h
HEADERS += DocumentDrawer/PageDrawer/Shapes.h

SOURCES += DocumentDrawer/PageDrawer/Properties/Properties.cpp
HEADERS += DocumentDrawer/PageDrawer/Properties/Properties.h

SOURCES += DocumentDrawer/PageDrawer/Shape/Shape.cpp
HEADERS += DocumentDrawer/PageDrawer/Shape/Shape.h

SOURCES += DocumentDrawer/PageDrawer/Shape/Arrow/Arrow.cpp
HEADERS += DocumentDrawer/PageDrawer/Shape/Arrow/Arrow.h

SOURCES += DocumentDrawer/PageDrawer/Shape/Ellipse/Ellipse.cpp
HEADERS += DocumentDrawer/PageDrawer/Shape/Ellipse/Ellipse.h

SOURCES += DocumentDrawer/PageDrawer/Shape/Line/Line.cpp
HEADERS += DocumentDrawer/PageDrawer/Shape/Line/Line.h

SOURCES += DocumentDrawer/PageDrawer/Shape/Rectangle/Rectangle.cpp
HEADERS += DocumentDrawer/PageDrawer/Shape/Rectangle/Rectangle.h

SOURCES += DocumentDrawer/PageDrawer/Shape/Text/Text.cpp
HEADERS += DocumentDrawer/PageDrawer/Shape/Text/Text.h
