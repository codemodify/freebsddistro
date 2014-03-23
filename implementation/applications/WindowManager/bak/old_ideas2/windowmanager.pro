################################################################################
#	Extern libraries
################################################################################

LIB_COMMON_linker	= -lcommon -L/usr/privateDocs/devel/projects/mybsd/src/common/src/out
LIB_COMMON_include	= /usr/privateDocs/devel/projects/mybsd/src/common/src

X11_linker	= 
X11_include	= /usr/X11R6/include



################################################################################
#	The QT-configuration for our application
################################################################################

TEMPLATE = app

CONFIG += qt debug warn_on

DEPENDPATH += .

INCLUDEPATH += . out $$LIB_COMMON_include $$X11_include

DESTDIR = out
OBJECTS_DIR = out
MOC_DIR = out
UI_DIR = out

TARGET = windowmanager

LIBS += $$LIB_COMMON_linker 



#	src for main entry point
SOURCES	+=	main.cpp



#	all the sources
SOURCES	+= windowmanager/windowmanager.cpp
HEADERS	+= windowmanager/windowmanager.h



#SOURCES += windowmanagerwindow/titlebar/titlebar.cpp
#HEADERS += windowmanagerwindow/titlebar/titlebar.h

#SOURCES += windowmanagerwindow/bottombar/enhancedqlabel/enhancedqlabel.cpp
#HEADERS += windowmanagerwindow/bottombar/enhancedqlabel/enhancedqlabel.h
#SOURCES += windowmanagerwindow/bottombar/bottombar.cpp
#HEADERS += windowmanagerwindow/bottombar/bottombar.h

#SOURCES	+= windowmanagerpanel/windowmanagerpanel.cpp
#HEADERS	+= windowmanagerpanel/windowmanagerpanel.h



SOURCES += xwindow/xwindow.cpp
HEADERS += xwindow/xwindow.h


SOURCES	+= xlibwrapper/xlibwrapper.cpp
HEADERS	+= xlibwrapper/xlibwrapper.h

