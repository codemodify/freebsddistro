################################################################################
#	Extern libraries
################################################################################

X11_linker	= 
X11_include	= /usr/X11R6/include



################################################################################
#	The QT-configuration for our application
################################################################################

TEMPLATE = app

CONFIG += qt debug warn_on

DEPENDPATH += .

INCLUDEPATH += . out $$X11_include

DESTDIR = out
OBJECTS_DIR = out
MOC_DIR = out
UI_DIR = out

TARGET = windowmanager

LIBS += 



#	src for main entry point
SOURCES	+=	main.cpp



#	all the sources

SOURCES	+= defaults/defaults.cpp
HEADERS	+= defaults/defaults.h

SOURCES	+= windowmanager/windowmanager.cpp
HEADERS	+= windowmanager/windowmanager.h

SOURCES	+= rubber/rubber.cpp
HEADERS	+= rubber/rubber.h

SOURCES	+= wborder/wborder.cpp
HEADERS	+= wborder/wborder.h

SOURCES	+= xwindow/xwindow.cpp
HEADERS	+= xwindow/xwindow.h

