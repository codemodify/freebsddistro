
################################################################################
ADDITIONAL_INCLUDES = /usr/home/nicu/workspace/webcontrols/src

################################################################################
TEMPLATE = lib
CONFIG += plugin
QT += 

# keep all temporary files separately
DESTDIR = out
OBJECTS_DIR = out
MOC_DIR = out
UI_DIR = out

INCLUDEPATH += . .. out $$ADDITIONAL_INCLUDES

DEPENDPATH += .

TARGET = nikushorite

LIBS +=

# sources for the application
HEADERS += nikushorsite.h
SOURCES += nikushorsite.cpp

WEBOBJECTS_FULL_PATH = $$ADDITIONAL_INCLUDES
include( $$ADDITIONAL_INCLUDES/webobjects.pro )

