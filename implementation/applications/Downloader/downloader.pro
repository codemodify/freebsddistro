###################################################################################################
#	Local variables

LIB_COMMON_linker	= -lcommon -L/usr/privateDocs/devel/projects/common/src/out
LIB_COMMON_include	= /usr/privateDocs/devel/projects/common/src



###################################################################################################
#	QT's variables

TEMPLATE = app

CONFIG += qt debug warn_on thread

DEPENDPATH += .

INCLUDEPATH += . $$LIB_COMMON_include ./out

DESTDIR = out
OBJECTS_DIR = out
MOC_DIR = out
UI_DIR = out

TARGET = downloader

LIBS += $$LIB_COMMON_linker

# src for main entry point
SOURCES +=		main.cpp
HEADERS +=		supportedtypes.h

# src for gui part
FORMS +=			downloader/downloader.ui	
SOURCES +=		downloader/downloader.cpp
HEADERS +=		downloader/downloader.h


# src for HTTP/FTP stuff
#SOURCES +=		urldownloaderthread/urldownloaderthread.cpp		urldownloaderthread/libcurldownload.cpp
#HEADERS +=		urldownloaderthread/urldownloaderthread.h		urldownloaderthread/libcurldownload.h


# src for HTTP/FTP stuff
SOURCES +=		torrentdownloader/torrentdownloader.cpp
HEADERS +=		torrentdownloader/torrentdownloader.h
