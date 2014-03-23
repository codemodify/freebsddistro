
# This file contains a set of variales that are used by the child projects
###########################################################################################

CONFIG     += debug          # project configuration and compiler options: 'release' / 'debug'
CONFIG     += warn_on        # compiler should output as many warnings as possible

DESTDIR     = $$PWD/../out/bin             # specifies where to put the target file.
MOC_DIR     = $$PWD/../out/temp_$$TARGET   # intermediate moc files should be placed.
OBJECTS_DIR = $$PWD/../out/temp_$$TARGET   # intermediate objects should be placed.
RCC_DIR     = $$PWD/../out/temp_$$TARGET   # intermediate resource files should be placed.
UI_DIR      = $$PWD/../out/temp_$$TARGET   # intermediate files from uic should be placed.

VERSION     = digger         # version

# this is handy to keep here otherwise all projects will have to add this by hand
INCLUDEPATH +=   $$PWD/../out/temp_$$TARGET
INCLUDEPATH +=   $$PWD/../blocks
INCLUDEPATH +=   $$PWD/../server

LIBS        += -L$$PWD/../out/bin
