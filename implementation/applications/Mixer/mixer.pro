###################################################################################################
#	QT's variables

TEMPLATE = app

CONFIG += qt debug warn_on thread

DEPENDPATH += .

INCLUDEPATH += . out

DESTDIR = out
OBJECTS_DIR = out
MOC_DIR = out
UI_DIR = out

TARGET = mixer



#	Src for main entry point
SOURCES	+=	main.cpp

#	Src for GUI part
FORMS	+=	mixer/mixer.ui
SOURCES	+=	mixer/mixer.cpp
HEADERS	+=	mixer/mixer.h
