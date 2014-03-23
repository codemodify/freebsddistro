TEMPLATE = app

CONFIG += qt debug warn_on thread

ADITIONAL_LIB_PATH = /usr/my_docs/devel/projects/common/src

DESTDIR = out
OBJECTS_DIR = out
MOC_DIR = out
UI_DIR = out

TARGET = filemanager

# required by all applications if they intend to use baloon-notifications
DEPENDPATH += $$ADITIONAL_LIB_PATH
include($$ADITIONAL_LIB_PATH/baloonnotificationmanager.pro)


INCLUDEPATH += . $$ADITIONAL_LIB_PATH

LIBS += -lcommon -L$$ADITIONAL_LIB_PATH/out 

# the entry point
SOURCES += main.cpp

# sources for filemanager (the gui part)
HEADERS += filemanager/filemanager.h
SOURCES += filemanager/filemanager.cpp
FORMS += filemanager/filemanager.ui

# sources for CopyManager
HEADERS += copymanager/copymanager.h
SOURCES += copymanager/copymanager.cpp

# sources for Files side
HEADERS += files/filesmodel/filesmodel.h		files/filesview/filesview.h
SOURCES += files/filesmodel/filesmodel.cpp	files/filesview/filesview.cpp
