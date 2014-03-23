TEMPLATE = app

CONFIG += qt debug warn_on

COMMON_PATH = /usr/privateDocs/devel/projects/common/src

INCLUDEPATH += . $$COMMON_PATH

LIBS += -lcommon -L$$COMMON_PATH/out

# the entry point
SOURCES += main.cpp
