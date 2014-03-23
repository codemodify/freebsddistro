###################################################################################################
#       Extern Lybraries - local variables

LIB_COMMON_linker       = -lcommon -L/usr/privateDocs/devel/projects/common/src/out
LIB_COMMON_include      = /usr/privateDocs/devel/projects/common/src



###################################################################################################
#       QT's variables

TEMPLATE = app

CONFIG += qt debug warn_on thread

DESTDIR = out
OBJECTS_DIR = out
MOC_DIR = out
UI_DIR = out

TARGET = cd-dvd-authoring

INCLUDEPATH += . $$LIB_COMMON_include

LIBS += $$LIB_COMMON_linker




# the entry point
SOURCES += main.cpp




# sources for main interface
HEADERS += cd-dvd-authoring/cd-dvd-authoring.h
SOURCES += cd-dvd-authoring/cd-dvd-authoring.cpp
FORMS += cd-dvd-authoring/cd-dvd-authoring.ui




# sources related to common part of code between the rest of the UI
HEADERS += common/commonui.h
SOURCES += common/commonui.cpp
FORMS += common/commonui.ui

HEADERS += common/commoncode.h
SOURCES += common/commoncode.cpp



# sources related to Optical Disc Image operations
HEADERS += opticaldiscimage/burner/burner.h
SOURCES += opticaldiscimage/burner/burner.cpp

HEADERS += opticaldiscimage/converter/converter.h
SOURCES += opticaldiscimage/converter/converter.cpp

HEADERS += opticaldiscimage/creator/creator.h
SOURCES += opticaldiscimage/creator/creator.cpp
FORMS += opticaldiscimage/creator/creator.ui

HEADERS += opticaldiscimage/hashgenerator/hashgenerator.h
SOURCES += opticaldiscimage/hashgenerator/hashgenerator.cpp




# sources realated to Optical Disc operations
HEADERS += opticaldisc/clonner/clonner.h
SOURCES += opticaldisc/clonner/clonner.cpp

HEADERS += opticaldisc/eraser/eraser.h
SOURCES += opticaldisc/eraser/eraser.cpp

HEADERS += opticaldisc/tester/tester.h
SOURCES += opticaldisc/tester/tester.cpp

