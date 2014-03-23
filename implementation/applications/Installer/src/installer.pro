
TEMPLATE = app

CONFIG += qt debug

QT += xml

DEPENDPATH += .

INCLUDEPATH += . out

DESTDIR = out
OBJECTS_DIR = out
MOC_DIR = out
UI_DIR = out

TARGET = installer

LIBS +=  

#	src for main entry point
SOURCES	+=	main.cpp


#	src for the webserver engine
SOURCES	+=	coreinstaller/coreinstaller.cpp
HEADERS	+=	coreinstaller/coreinstaller.h
