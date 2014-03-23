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

CONFIG += qt

DEPENDPATH += .

INCLUDEPATH += . out $$X11_include

DESTDIR = ../out
OBJECTS_DIR = ../out
MOC_DIR = ../out
UI_DIR = ../out

TARGET = windowmanager

LIBS += 



#	src for main entry point
SOURCES	+=	main.cpp



#	sources for the core WM engine
SOURCES	+= windowmanager/windowmanager.cpp
HEADERS	+= windowmanager/windowmanager.h

#	sources for a X11 window wrapper
SOURCES += xwindow/xwindow.cpp
HEADERS += xwindow/xwindow.h

#	sources for a taskbar, aimed to facilitate the work with applications
SOURCES += taskbar/taskbar.cpp
HEADERS += taskbar/taskbar.h
SOURCES += taskbar/enhancedqpushbutton/enhancedqpushbutton.cpp
HEADERS += taskbar/enhancedqpushbutton/enhancedqpushbutton.h

#	sources for some useful stuff used in code above to simplify the 
#	support and to demonstrate the quality including the design
SOURCES	+= helpers/xlibwrapper/xlibwrapper.cpp
HEADERS	+= helpers/xlibwrapper/xlibwrapper.h
HEADERS += helpers/xlibwrapper/x11agreements/agreements.h
HEADERS += helpers/xlibwrapper/x11agreements/iccc.h
HEADERS += helpers/xlibwrapper/x11agreements/ewmh.h

SOURCES += helpers/enhancedqwidget/enhancedqwidget.cpp
HEADERS += helpers/enhancedqwidget/enhancedqwidget.h

