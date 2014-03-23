
# Set the configuration for these sources
#########################################################################
TEMPLATE     = lib
TARGET       = Architector.Renderer.2DRenderer
CONFIG      += qt thread shared
QT           = core gui

INCLUDEPATH += $$PWD/../Architector


# Set output for all intermediate files to one place
##########################################################################
include( ../../_builder_/common-options.pro )


# Settle what sources to use
##########################################################################
HEADERS += Architecture.Renderer.RendererFactory.h
SOURCES += Architecture.Renderer.RendererFactory.cpp

HEADERS += Architecture.Renderer.RendererGraphicsScene.h
SOURCES += Architecture.Renderer.RendererGraphicsScene.cpp

HEADERS += Architecture.Renderer.RendererGraphicsView.h
SOURCES += Architecture.Renderer.RendererGraphicsView.cpp

HEADERS += Architecture.Renderer.PropertiesRenderer.h
SOURCES += Architecture.Renderer.PropertiesRenderer.cpp

HEADERS += Architecture.Renderer.ArrowRenderer.h
SOURCES += Architecture.Renderer.ArrowRenderer.cpp
HEADERS += Architecture.Renderer.ArrowDoubleRenderer.h
SOURCES += Architecture.Renderer.ArrowDoubleRenderer.cpp
HEADERS += Architecture.Renderer.TextRenderer.h
SOURCES += Architecture.Renderer.TextRenderer.cpp
HEADERS += Architecture.Renderer.InterfaceRenderer.h
SOURCES += Architecture.Renderer.InterfaceRenderer.cpp
HEADERS += Architecture.Renderer.AbstractClassRenderer.h
SOURCES += Architecture.Renderer.AbstractClassRenderer.cpp
HEADERS += Architecture.Renderer.ClassRenderer.h
SOURCES += Architecture.Renderer.ClassRenderer.cpp
