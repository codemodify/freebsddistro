
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

TARGET = ssh-server

LIBS += -l $$ADDITIONAL_LIBS -L $$ADDITIONAL_LIBS_PATH

# sources for the application
HEADERS += sshserver/sshserver.h
SOURCES += sshserver/sshserver.cpp

HEADERS += sshserver/configuration.h
