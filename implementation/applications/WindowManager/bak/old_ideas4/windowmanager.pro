
TEMPLATE = app

CONFIG += qt

DESTDIR = out
OBJECTS_DIR = out
MOC_DIR = out
UI_DIR = out

TARGET = windowmanager

INCLUDEPATH += . out
DEPENDPATH += . out


# sources
SOURCES += main.cpp

SOURCES += main/main.cpp
HEADERS += main/main.h

SOURCES += client/client.cpp
HEADERS += client/client.h
SOURCES += client/resizeframe/resizeframe.cpp
HEADERS += client/resizeframe/resizeframe.h

SOURCES += manage/manage.cpp
HEADERS += manage/manage.h

