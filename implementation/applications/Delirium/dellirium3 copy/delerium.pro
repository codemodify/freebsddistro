TEMPLATE = app

CONFIG += qt

QT += xml network

DEPENDPATH += .

INCLUDEPATH += . out

DESTDIR = out
OBJECTS_DIR = out
MOC_DIR = out
UI_DIR = out

TARGET = dellirium

#       src for main entry point
SOURCES +=      main.cpp

#       all the sources
SOURCES += dellirium/dellirium.cpp
HEADERS += dellirium/dellirium.h

SOURCES += mapeditor/mapeditor.cpp
HEADERS += mapeditor/mapeditor.h

SOURCES += client/client.cpp
HEADERS += client/client.h

SOURCES += server/server.cpp
HEADERS += server/server.h

SOURCES += 2drenderer/2drenderer.cpp
HEADERS += 2drenderer/2drenderer.h

# game objects
SOURCES += objects/gameobject.cpp
HEADERS += objects/gameobject.h

SOURCES += objects/neutralbush/neutralbush.cpp
HEADERS += objects/neutralbush/neutralbush.h
SOURCES += objects/neutralwall/neutralwall.cpp
HEADERS += objects/neutralwall/neutralwall.h

SOURCES += objects/meball/meball.cpp
HEADERS += objects/meball/meball.h
SOURCES += objects/metarget/metarget.cpp
HEADERS += objects/metarget/metarget.h

SOURCES += objects/enemyball/enemyball.cpp
HEADERS += objects/enemyball/enemyball.h
SOURCES += objects/enemytarget/enemytarget.cpp
HEADERS += objects/enemytarget/enemytarget.h
