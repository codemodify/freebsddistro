X11_include = /usr/X11R6/include


INCLUDEPATH += ../. ../../.  $$X11_include


TARGET = ewmh


# test application
SOURCES += main.cpp


SOURCES += ../../x11agreements/ewmh/ewmh.cpp
HEADERS += ../../x11agreements/ewmh/ewmh.h

