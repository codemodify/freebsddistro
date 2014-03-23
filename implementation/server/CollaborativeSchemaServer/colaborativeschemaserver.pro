
# some additional variables
##########################################################################
ADDITIONAL_INLCUDES = /usr/home/nicu/sdk
ADDITIONAL_LIBS_PATH = /usr/home/nicu/sdk/libs 
ADDITIONAL_LIBS = isp-library


################################################################################
TEMPLATE = lib
CONFIG += plugin
QT += xml

# keep all temporary files separately
DESTDIR = out
OBJECTS_DIR = out
MOC_DIR = out
UI_DIR = out

DEPENDPATH += .

INCLUDEPATH += . .. out $$ADDITIONAL_INLCUDES

TARGET = collaborativeschema-server

LIBS += -l $$ADDITIONAL_LIBS -L $$ADDITIONAL_LIBS_PATH

# sources for the application
HEADERS += collaborativeschemaserver/collaborativeschemaserver.h
SOURCES += collaborativeschemaserver/collaborativeschemaserver.cpp

HEADERS += collaborativeschemaserver/configuration.h
