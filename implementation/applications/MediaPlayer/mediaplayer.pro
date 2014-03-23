###################################################################################################
#	Extern Lybraries - local variables

LIB_COMMON_linker	= -lcommon -L/usr/privateDocs/devel/projects/mybsd/src/common/src/out
LIB_COMMON_include	= /usr/privateDocs/devel/projects/mybsd/src/common/src



###################################################################################################
#	QT's variables

TEMPLATE = app

CONFIG += qt debug warn_on thread

QT += xml

DEPENDPATH += .

INCLUDEPATH += . out mplayerwrapper $$LIB_COMMON_include

DESTDIR = out
OBJECTS_DIR = out
MOC_DIR = out
UI_DIR = out

TARGET = mediaplayer

LIBS += $$LIB_COMMON_linker

#	Src for main entry point
SOURCES	+=	main.cpp


#	Src for mplayer-wrapper
SOURCES +=	mplayerwrapper/mplayerwrapper.cpp
HEADERS +=	mplayerwrapper/mplayerwrapper.h

#	Src for GUI part
FORMS	+=	mediaplayer/mediaplayer.ui
SOURCES	+=	mediaplayer/mediaplayer.cpp
HEADERS	+=	mediaplayer/mediaplayer.h
