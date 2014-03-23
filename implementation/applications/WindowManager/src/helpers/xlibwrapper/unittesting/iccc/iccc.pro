X11_include = /usr/X11R6/include


INCLUDEPATH += ../. ../../.  $$X11_include


TARGET = iccc


# test application
SOURCES += main.cpp


SOURCES += ../../x11agreements/iccc/iccc.cpp
HEADERS += ../../x11agreements/iccc/iccc.h

