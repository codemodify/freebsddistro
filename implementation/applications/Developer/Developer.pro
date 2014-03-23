
# External library dependencies
##########################################################################
include( ../../blocks/IconProvider/IconProvider.pri                     )
include( ../../blocks/Styling/Styling.pri                               )
include( ../../blocks/Settings/Settings.pri                             )
include( ../../blocks/Configurator/Configurator.pri                     )
include( ../../blocks/ConfiguratorReader/ConfiguratorReader.pri         )
include( ../../blocks/FileSystemOperations/FileSystemOperations.pri     )
include( ../../blocks/FileSystemOperationsUi/FileSystemOperationsUi.pri )
include( ../../blocks/FileTreeView/FileTreeView.pri                     )
include( ../../blocks/FileSearch/FileSearch.pri                         )
include( ../../blocks/FileSearchUi/FileSearchUi.pri                     )
include( ../../blocks/FileEditor/FileEditor.pri                         )
include( ../../blocks/FileLauncher/FileLauncher.pri                     )
include( ../../blocks/Deployer/Deployer.pri                             )
include( ../../blocks/CustomDockWidget/CustomDockWidget.pri             )


# Set the configuration for these sources
#########################################################################
TEMPLATE     = app
TARGET       = Developer
CONFIG      += qt thread
QT           = core gui xml webkit


# Set output for all intermediate files to one place
##########################################################################
include( ../../_builder_/common-options.pro )


# Settle what sources to use
##########################################################################
SOURCES += main.cpp

HEADERS += Developer.h
SOURCES += Developer.cpp

HEADERS += View/View.h
SOURCES += View/View.cpp

HEADERS += View/FileSystemBrowser/FileSystemBrowser.h
SOURCES += View/FileSystemBrowser/FileSystemBrowser.cpp

HEADERS += View/WorkspaceNavigator/WorkspaceNavigator.h
SOURCES += View/WorkspaceNavigator/WorkspaceNavigator.cpp
HEADERS += View/WorkspaceNavigator/WorkspaceFile.h
SOURCES += View/WorkspaceNavigator/WorkspaceFile.cpp
HEADERS += View/WorkspaceNavigator/ProjectProperties/ProjectProperties.h
SOURCES += View/WorkspaceNavigator/ProjectProperties/ProjectProperties.cpp
FORMS   += View/WorkspaceNavigator/ProjectProperties/ProjectProperties.ui

HEADERS += View/Editor/Editor.h
SOURCES += View/Editor/Editor.cpp

HEADERS += View/OutputConsole/OutputConsole.h
SOURCES += View/OutputConsole/OutputConsole.cpp

HEADERS += View/SearchResult/SearchResult.h
SOURCES += View/SearchResult/SearchResult.cpp
