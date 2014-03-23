
# Set the configuration for these sources
##########################################################################
TEMPLATE     = lib
TARGET       = FileEditor
CONFIG      += qt thread shared
QT           = core gui


# include common config stuff
##########################################################################
include( ../../_builder_/common-options.pro )


# Settle what sources to use
##########################################################################
HEADERS += FileEditor.h
SOURCES += FileEditor.cpp

HEADERS += FileEditor.Content.h
SOURCES += FileEditor.Content.cpp

SOURCES	+= LineNumbers/LineNumbers.cpp
HEADERS	+= LineNumbers/LineNumbers.h

SOURCES	+= ToolWidget/ToolWidget.cpp
HEADERS	+= ToolWidget/ToolWidget.h
SOURCES	+= ToolWidget/TextFinder/TextFinder.cpp
HEADERS	+= ToolWidget/TextFinder/TextFinder.h

SOURCES	+= TextFileEditor/TextFileEditor.cpp
HEADERS	+= TextFileEditor/TextFileEditor.h
SOURCES	+= TextFileEditor/TabSettings.cpp
HEADERS	+= TextFileEditor/TabSettings.h

SOURCES	+= TextFileEditor/SyntaxHighlighter/SyntaxHighlighter.cpp
HEADERS	+= TextFileEditor/SyntaxHighlighter/SyntaxHighlighter.h

RESOURCES += FileEditor.qrc

