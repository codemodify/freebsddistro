
# External library dependencies
##########################################################################
include( ../../blocks/IconProvider/IconProvider.pri                 )
include( ../../blocks/Styling/Styling.pri                           )
include( ../../blocks/Settings/Settings.pri                         )
include( ../../blocks/Configurator/Configurator.pri                 )
include( ../../blocks/ConfiguratorReader/ConfiguratorReader.pri     )
include( ../../blocks/Pluginer/Pluginer.pri                         )
include( ../../blocks/PluginerUi/PluginerUi.pri                     )


# Set the configuration for these sources
#########################################################################
TEMPLATE     = app
TARGET       = Architector
CONFIG      += qt thread
QT           = core gui webkit


# Set output for all intermediate files to one place
##########################################################################
include( ../../_builder_/common-options.pro )


# Settle what sources to use
##########################################################################
SOURCES += main.cpp

HEADERS += Architector.h
SOURCES += Architector.cpp

HEADERS += Architecture.Ui.h
SOURCES += Architecture.Ui.cpp

HEADERS += Architecture.Shapes.h
SOURCES += Architecture.Shapes.cpp
HEADERS += Architecture.Shapes.IShape.h
HEADERS += Architecture.Shapes.AbstractShape.h
SOURCES += Architecture.Shapes.AbstractShape.cpp
HEADERS += Architecture.Shapes.ArrowDouble.h
SOURCES += Architecture.Shapes.ArrowDouble.cpp
HEADERS += Architecture.Shapes.Arrow.h
SOURCES += Architecture.Shapes.Arrow.cpp
HEADERS += Architecture.Shapes.Interface.h
SOURCES += Architecture.Shapes.Interface.cpp
HEADERS += Architecture.Shapes.AbstractClass.h
SOURCES += Architecture.Shapes.AbstractClass.cpp
HEADERS += Architecture.Shapes.Class.h
SOURCES += Architecture.Shapes.Class.cpp
HEADERS += Architecture.Shapes.Properties.h
SOURCES += Architecture.Shapes.Properties.cpp
HEADERS += Architecture.Shapes.Text.h
SOURCES += Architecture.Shapes.Text.cpp

HEADERS += Architecture.Renderer.IRendererFactory.h
HEADERS += Architecture.Renderer.IAbstractClassRenderer.h
HEADERS += Architecture.Renderer.IArrowDoubleRenderer.h
HEADERS += Architecture.Renderer.IArrowRenderer.h
HEADERS += Architecture.Renderer.IClassRenderer.h
HEADERS += Architecture.Renderer.IInterfaceRenderer.h
HEADERS += Architecture.Renderer.IPropertiesRenderer.h
HEADERS += Architecture.Renderer.ITextRenderer.h
