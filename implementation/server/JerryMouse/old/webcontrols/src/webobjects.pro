
#############################################################################################
# this variable here needs to be defined in a project that will include these controls
isEmpty( WEBOBJECTS_FULL_PATH ) {
	WEBOBJECTS_FULL_PATH = .
}

#	src for the controls
SOURCES	+= $$WEBOBJECTS_FULL_PATH/webobjects/webobject.cpp
HEADERS	+= $$WEBOBJECTS_FULL_PATH/webobjects/webobject.h

SOURCES	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/webpage.cpp
HEADERS	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/webpage.h




SOURCES	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/pageobject.cpp
HEADERS	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/pageobject.h

SOURCES	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/button/button.cpp
HEADERS	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/button/button.h

SOURCES	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/checkbox/checkbox.cpp
HEADERS	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/checkbox/checkbox.h

SOURCES	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/groupbox/groupbox.cpp
HEADERS	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/groupbox/groupbox.h

SOURCES	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/label/label.cpp
HEADERS	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/label/label.h

SOURCES	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/layout/layout.cpp
HEADERS	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/layout/layout.h

SOURCES	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/layout/horizontal/horizontal.cpp
HEADERS	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/layout/horizontal/horizontal.h

SOURCES	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/layout/vertical/vertical.cpp
HEADERS	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/layout/vertical/vertical.h




SOURCES	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/htmlobject.cpp
HEADERS	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/htmlobject.h

SOURCES	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/a/a.cpp
HEADERS	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/a/a.h

SOURCES	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/b/b.cpp
HEADERS	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/b/b.h

SOURCES	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/br/br.cpp
HEADERS	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/br/br.h

SOURCES	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/button/button.cpp
HEADERS	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/button/button.h

SOURCES	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/check/check.cpp
HEADERS	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/check/check.h

SOURCES	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/del/del.cpp
HEADERS	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/del/del.h

SOURCES	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/edit/edit.cpp
HEADERS	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/edit/edit.h

SOURCES	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/fieldset/fieldset.cpp
HEADERS	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/fieldset/fieldset.h

SOURCES	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/font/font.cpp
HEADERS	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/font/font.h

SOURCES	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/form/form.cpp
HEADERS	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/form/form.h

SOURCES	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/hr/hr.cpp
HEADERS	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/hr/hr.h

SOURCES	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/i/i.cpp
HEADERS	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/i/i.h

SOURCES	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/img/img.cpp
HEADERS	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/img/img.h

SOURCES	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/password/password.cpp
HEADERS	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/password/password.h

SOURCES	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/radio/radio.cpp
HEADERS	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/radio/radio.h

SOURCES	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/table/table.cpp
HEADERS	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/table/table.h

SOURCES	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/tagbody/tagbody.cpp
HEADERS	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/tagbody/tagbody.h

SOURCES	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/taghead/taghead.cpp
HEADERS	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/taghead/taghead.h

SOURCES	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/taghtml/taghtml.cpp
HEADERS	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/taghtml/taghtml.h

SOURCES	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/tagtitle/tagtitle.cpp
HEADERS	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/tagtitle/tagtitle.h

SOURCES	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/tbody/tbody.cpp
HEADERS	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/tbody/tbody.h

SOURCES	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/td/td.cpp
HEADERS	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/td/td.h

SOURCES	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/text/text.cpp
HEADERS	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/text/text.h

SOURCES	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/tfoot/tfoot.cpp
HEADERS	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/tfoot/tfoot.h

SOURCES	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/thead/thead.cpp
HEADERS	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/thead/thead.h

SOURCES	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/tr/tr.cpp
HEADERS	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/tr/tr.h

SOURCES	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/u/u.cpp
HEADERS	+= $$WEBOBJECTS_FULL_PATH/webobjects/webpage/pageobject/htmlelements/u/u.h
