
# External library dependencies
##########################################################################
EXTERNAL_DEPENDECY_NAME__0001 = Styling
EXTERNAL_DEPENDECY_LIBS__0001 = -l$$EXTERNAL_DEPENDECY_NAME__0001

EXTERNAL_DEPENDECY_NAME__0002 = CustomDockWidget
EXTERNAL_DEPENDECY_LIBS__0002 = -l$$EXTERNAL_DEPENDECY_NAME__0002

LIBS += $$EXTERNAL_DEPENDECY_LIBS__0001
LIBS += $$EXTERNAL_DEPENDECY_LIBS__0002


# This library
##########################################################################
THIS_DEPENDECY_NAME = FileEditor
THIS_DEPENDECY_LIBS = -l$$THIS_DEPENDECY_NAME

LIBS += $$THIS_DEPENDECY_LIBS
