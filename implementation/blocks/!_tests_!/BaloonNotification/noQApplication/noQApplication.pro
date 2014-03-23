TEMPLATE = app

CONFIG += qt debug warn_on

NON_QT_VARIABLE_LIB_PATH = /usr/my_docs/devel/projects/common/src

INCLUDEPATH += . $$NON_QT_VARIABLE_LIB_PATH

LIBS += -lcommon -L$$NON_QT_VARIABLE_LIB_PATH/out

# required by all applications if they intend to use baloon-notifications
DEPENDPATH += $$NON_QT_VARIABLE_LIB_PATH
include($$NON_QT_VARIABLE_LIB_PATH/baloonnotificationmanager.pro)

# the entry point
SOURCES += main.cpp
