
# External headers / libraries
##########################################################################
EXTERNAL_DEPENDECY_NAME__0001 =
EXTERNAL_DEPENDECY_LIBS__0001 =


# Set the configuration for these sources
#########################################################################
TEMPLATE     = lib
TARGET       = MyForm
CONFIG      += qt thread shared warn_on release
QT           = core gui
LIBS        += $$EXTERNAL_DEPENDECY_LIBS__0001

DESTDIR     = out
MOC_DIR     = out
OBJECTS_DIR = out
RCC_DIR     = out
UI_DIR      = out


# Set output for all intermediate files to one place
##########################################################################
include( ../applications.pro )


# Settle what sources to use
##########################################################################
SOURCES += main.cpp

HEADERS += MyForm.h
SOURCES += MyForm.cpp

