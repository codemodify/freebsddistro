
# External library dependencies
##########################################################################
EXTERNAL_DEPENDECY_NAME__0001 = FileSystemOperations
EXTERNAL_DEPENDECY_LIBS__0001 = -l$$EXTERNAL_DEPENDECY_NAME__0001

LIBS += $$EXTERNAL_DEPENDECY_LIBS__0001


# This library
##########################################################################
THIS_DEPENDECY_NAME = FileSystemOperationsUi
THIS_DEPENDECY_LIBS = -l$$THIS_DEPENDECY_NAME

LIBS += $$THIS_DEPENDECY_LIBS
