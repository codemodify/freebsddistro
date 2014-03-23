X11_include = /usr/X11R6/include

TEMPLATE = app

CONFIG += qt debug

INCLUDEPATH += ../. ../../.  $$X11_include


TARGET = xlibwrapper


# test application
SOURCES += main.cpp

SOURCES += gui/gui.cpp
HEADERS += gui/gui.h
FORMS   += gui/gui.ui



# sources for the tested code

SOURCES += ../../x11agreements/iccc/iccc.cpp
HEADERS += ../../x11agreements/iccc/iccc.h

SOURCES += ../../x11agreements/ewmh/ewmh.cpp
HEADERS += ../../x11agreements/ewmh/ewmh.h

HEADERS += ../../x11agreements/x11agreements.h

SOURCES += ../../xlibwrapper.cpp
HEADERS += ../../xlibwrapper.h

