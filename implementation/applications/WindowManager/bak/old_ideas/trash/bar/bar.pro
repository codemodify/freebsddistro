TEMPLATE = app

CONFIG += qt debug warn_on

DEPENDPATH += .

INCLUDEPATH += . out /usr/privateDocs/devel/projects/common/src /usr/X11R6/include

DESTDIR = out
OBJECTS_DIR = out
MOC_DIR = out
UI_DIR = out

TARGET = bar

LIBS += -L/usr/privateDocs/devel/projects/common/src/out -lcommon 

#	src for main entry point
SOURCES	+=	main.cpp


#	src for GUI part
FORMS	+=	bar/bar.ui
SOURCES	+=	bar/bar.cpp
HEADERS	+=	bar/bar.h

#	enhanced widget
SOURCES	+=	bar/enhancedQWidget/enhancedqwidget.cpp
HEADERS	+=	bar/enhancedQWidget/enhancedqwidget.h
