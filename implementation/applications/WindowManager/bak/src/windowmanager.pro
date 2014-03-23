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
SOURCES	+= appbar/apbar.cpp
HEADERS	+= appbar/apbar.h

SOURCES	+= defaults/defaults.cpp
HEADERS	+= defaults/defaults.h

SOURCES	+= keyboard/keyboard.cpp
HEADERS	+= keyboard/keyboard.h

SOURCES	+= menu/menu.cpp
HEADERS	+= menu/menu.h

SOURCES	+= pager/pager.cpp
HEADERS	+= pager/pager.h

SOURCES	+= procbar/procbar.cpp
HEADERS	+= procbar/procbar.h

SOURCES	+= qapp/qapp.cpp
HEADERS	+= qapp/qapp.h

SOURCES	+= rubber/rubber.cpp
HEADERS	+= rubber/rubber.h

SOURCES	+= toolbar/toolbar.cpp
HEADERS	+= toolbar/toolbar.h

SOURCES	+= wborder/wborder.cpp
HEADERS	+= wborder/wborder.h

SOURCES	+= wininfo/winfo.cpp
HEADERS	+= wininfo/winfo.h

SOURCES	+= winlist/winlist.cpp
HEADERS	+= winlist/winlist.h

SOURCES	+= xwindow/xwindow.cpp
HEADERS	+= xwindow/xwindow.h

HEADERS	+= conf.h
HEADERS	+= defs.h
HEADERS	+= eventnames.h

