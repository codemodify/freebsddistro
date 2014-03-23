###################################################################################################
#	Extern Lybraries - local variables

LIB_COMMON_linker	= -lcommon -L/Users/carabutnicolae/devel/libraries/binaries
LIB_COMMON_include	= /Users/carabutnicolae/devel/libraries/include

LIB_TLS_linker = ltls -L/
LIB_TLS_include = /

###################################################################################################
#	QT's variables

TEMPLATE = app

CONFIG += qt debug warn_on thread

QT += network xml

DESTDIR = out
OBJECTS_DIR = out
MOC_DIR = out
UI_DIR = out

TARGET = mailserver

INCLUDEPATH += .

LIBS += 

# the entry point
SOURCES += main.cpp

# the starting engine
SOURCES += mailserver/mailserver.cpp
HEADERS += mailserver/mailserver.h

# supported services
HEADERS += services/mailserverconfigvariables.h

SOURCES += services/smtp/smtp.cpp
HEADERS += services/smtp/smtp.h
SOURCES += services/smtp/smtpconnectionhandler/smtpconnectionhandler.cpp
HEADERS += services/smtp/smtpconnectionhandler/smtpconnectionhandler.h
SOURCES += services/smtp/smtpconnectionhandler/sendmail/sendmail.cpp
HEADERS += services/smtp/smtpconnectionhandler/sendmail/sendmail.h
SOURCES += services/smtp/smtpconnectionhandler/receivemail/receivemail.cpp
HEADERS += services/smtp/smtpconnectionhandler/receivemail/receivemail.h

SOURCES += services/pop/pop.cpp
HEADERS += services/pop/pop.h
SOURCES += services/pop/popconnectionhandler/popconnectionhandler.cpp
HEADERS += services/pop/popconnectionhandler/popconnectionhandler.h

# the integrated database engine
SOURCES += maildatabase/maildatabase.cpp
HEADERS += maildatabase/maildatabase.h

# the interface to the DNS system
SOURCES += dnsinfo/dnsinfo.cpp
HEADERS += dnsinfo/dnsinfo.h

SOURCES += dnsinfo/dnsresolver/dnsresolver.cpp
HEADERS += dnsinfo/dnsresolver/dnsresolver.h

SOURCES += dnsinfo/dnsresolver/message/message.cpp
HEADERS += dnsinfo/dnsresolver/message/message.h
SOURCES += dnsinfo/dnsresolver/message/utility.cpp
HEADERS += dnsinfo/dnsresolver/message/utility.h

SOURCES += dnsinfo/dnsresolver/message/header/header.cpp
HEADERS += dnsinfo/dnsresolver/message/header/header.h

SOURCES += dnsinfo/dnsresolver/message/question/question.cpp
HEADERS += dnsinfo/dnsresolver/message/question/question.h

SOURCES += dnsinfo/dnsresolver/message/resourcerecord/resourcerecord.cpp
HEADERS += dnsinfo/dnsresolver/message/resourcerecord/resourcerecord.h
