
ADDITIONAL_INCLUDES = ../../webcontrols/src
#####################################3

TEMPLATE = app

CONFIG += qt 

QT +=

DEPENDPATH += .

INCLUDEPATH += . out $$ADDITIONAL_INCLUDES

DESTDIR = out
OBJECTS_DIR = out
MOC_DIR = out
UI_DIR = out

TARGET = stress

LIBS +=  

#	src for main entry point
SOURCES	+=	main.cpp


#	src for the controls
SOURCES	+= ../../webcontrols/src/webobjects/webobject.cpp
HEADERS	+= ../../webcontrols/src/webobjects/webobject.h

SOURCES	+= ../../webcontrols/src/webobjects/webpage/webpage.cpp
HEADERS	+= ../../webcontrols/src/webobjects/webpage/webpage.h

SOURCES	+= ../../webcontrols/src/webobjects/webpage/pageobject/pageobject.cpp
HEADERS	+= ../../webcontrols/src/webobjects/webpage/pageobject/pageobject.h

SOURCES	+= ../../webcontrols/src/webobjects/webpage/pageobject/label/label.cpp
HEADERS	+= ../../webcontrols/src/webobjects/webpage/pageobject/label/label.h

SOURCES	+= ../../webcontrols/src/webobjects/webpage/pageobject/groupbox/groupbox.cpp
HEADERS	+= ../../webcontrols/src/webobjects/webpage/pageobject/groupbox/groupbox.h

SOURCES	+= ../../webcontrols/src/webobjects/webpage/pageobject/layout/layout.cpp
HEADERS	+= ../../webcontrols/src/webobjects/webpage/pageobject/layout/layout.h

SOURCES	+= ../../webcontrols/src/webobjects/webpage/pageobject/layout/horizontal/horizontal.cpp
HEADERS	+= ../../webcontrols/src/webobjects/webpage/pageobject/layout/horizontal/horizontal.h

SOURCES	+= ../../webcontrols/src/webobjects/webpage/pageobject/layout/vertical/vertical.cpp
HEADERS	+= ../../webcontrols/src/webobjects/webpage/pageobject/layout/vertical/vertical.h




SOURCES	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/htmlobject.cpp
HEADERS	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/htmlobject.h

SOURCES	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/a/a.cpp
HEADERS	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/a/a.h

SOURCES	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/b/b.cpp
HEADERS	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/b/b.h

SOURCES	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/br/br.cpp
HEADERS	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/br/br.h

SOURCES	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/button/button.cpp
HEADERS	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/button/button.h

SOURCES	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/check/check.cpp
HEADERS	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/check/check.h

SOURCES	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/del/del.cpp
HEADERS	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/del/del.h

SOURCES	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/edit/edit.cpp
HEADERS	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/edit/edit.h

SOURCES	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/fieldset/fieldset.cpp
HEADERS	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/fieldset/fieldset.h

SOURCES	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/font/font.cpp
HEADERS	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/font/font.h

SOURCES	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/form/form.cpp
HEADERS	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/form/form.h

SOURCES	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/hr/hr.cpp
HEADERS	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/hr/hr.h

SOURCES	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/i/i.cpp
HEADERS	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/i/i.h

SOURCES	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/img/img.cpp
HEADERS	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/img/img.h

SOURCES	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/password/password.cpp
HEADERS	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/password/password.h

SOURCES	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/radio/radio.cpp
HEADERS	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/radio/radio.h

SOURCES	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/table/table.cpp
HEADERS	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/table/table.h

SOURCES	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/tagbody/tagbody.cpp
HEADERS	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/tagbody/tagbody.h

SOURCES	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/taghead/taghead.cpp
HEADERS	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/taghead/taghead.h

SOURCES	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/taghtml/taghtml.cpp
HEADERS	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/taghtml/taghtml.h

SOURCES	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/tagtitle/tagtitle.cpp
HEADERS	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/tagtitle/tagtitle.h

SOURCES	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/tbody/tbody.cpp
HEADERS	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/tbody/tbody.h

SOURCES	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/td/td.cpp
HEADERS	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/td/td.h

SOURCES	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/text/text.cpp
HEADERS	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/text/text.h

SOURCES	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/tfoot/tfoot.cpp
HEADERS	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/tfoot/tfoot.h

SOURCES	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/thead/thead.cpp
HEADERS	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/thead/thead.h

SOURCES	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/tr/tr.cpp
HEADERS	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/tr/tr.h

SOURCES	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/u/u.cpp
HEADERS	+= ../../webcontrols/src/webobjects/webpage/pageobject/htmlelements/u/u.h
