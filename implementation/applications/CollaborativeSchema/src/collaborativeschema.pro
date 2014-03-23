
# some additional variables
##########################################################################
ADDITIONAL_INLCUDES = /usr/home/nicu/sdk
ADDITIONAL_LIBS_PATH = /usr/home/nicu/sdk/libs 
ADDITIONAL_LIBS = isp-library

##########################################################################
#	QT internal variables
##########################################################################
TEMPLATE = app
CONFIG += qt 
QT += network

DEPENDPATH += .

INCLUDEPATH += . out 
#$$ADDITIONAL_INLCUDES

DESTDIR = out
OBJECTS_DIR = out
MOC_DIR = out
UI_DIR = out

TARGET = collaborativeschema

LIBS += 
#-l $$ADDITIONAL_LIBS -L $$ADDITIONAL_LIBS_PATH

#	src for main entry point
SOURCES	+=	main.cpp

#	src for the webserver engine
SOURCES	+=	collaborativeschema/collaborativeschema.cpp
HEADERS	+=	collaborativeschema/collaborativeschema.h

SOURCES	+=	graphicscene/graphicscene.cpp
HEADERS	+=	graphicscene/graphicscene.h
