
# External library dependencies
##########################################################################
include( ../../blocks/FileSystemOperations/FileSystemOperations.pri     )
include( ../../blocks/FileSystemOperationsUi/FileSystemOperationsUi.pri )
include( ../../blocks/IconProvider/IconProvider.pri                     )
include( ../../blocks/FileListView/FileListView.pri                     )
include( ../../blocks/Settings/Settings.pri                             )
include( ../../blocks/FileSearch/FileSearch.pri                         )
include( ../../blocks/FileSearchUi/FileSearchUi.pri                     )
include( ../../blocks/FileEditor/FileEditor.pri                         )
include( ../../blocks/Styling/Styling.pri                               )
include( ../../blocks/FileLauncher/FileLauncher.pri                     )
include( ../../blocks/FileTreeView/FileTreeView.pri                     )
include( ../../blocks/Configurator/Configurator.pri                     )
include( ../../blocks/ConfiguratorReader/ConfiguratorReader.pri         )


# Set the configuration for these sources
#########################################################################
TEMPLATE     = app
TARGET       = Commander
CONFIG      += qt thread
QT           = core gui webkit


# Set output for all intermediate files to one place
##########################################################################
include( ../../_builder_/common-options.pro )


# Settle what sources to use
##########################################################################
SOURCES += main.cpp

HEADERS += Commander.h
SOURCES += Commander.cpp

HEADERS += Dialogs/AddRemoveSelection/AddRemoveSelection.h
SOURCES += Dialogs/AddRemoveSelection/AddRemoveSelection.cpp

HEADERS += TabWidget/TabWidget.h
SOURCES += TabWidget/TabWidget.cpp
HEADERS += TabWidget/TabContent/TabContent.h
SOURCES += TabWidget/TabContent/TabContent.cpp
HEADERS += TabWidget/TabContent/Filter/Filter.h
SOURCES += TabWidget/TabContent/Filter/Filter.cpp
HEADERS += TabWidget/TabContent/Filter/NameFilter/NameFilter.h
SOURCES += TabWidget/TabContent/Filter/NameFilter/NameFilter.cpp
HEADERS += TabWidget/TabContent/Filter/ContentFilter/ContentFilter.h
SOURCES += TabWidget/TabContent/Filter/ContentFilter/ContentFilter.cpp

HEADERS += Dialogs/MultiRenameTool/MultiRenameTool.h
SOURCES += Dialogs/MultiRenameTool/MultiRenameTool.cpp
