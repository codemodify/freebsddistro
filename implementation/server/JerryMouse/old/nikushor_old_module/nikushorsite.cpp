
#include "nikushorsite.h"

Q_EXPORT_PLUGIN2( nikushorSite, NikushorSite )

NikushorSite::NikushorSite():
	WebSite()
{}

NikushorSite::~NikushorSite()
{}

bool NikushorSite::initialize()
{
	return true;
}

void NikushorSite::shutdown()
{}

QString NikushorSite::getName()
{
	return "NikushorSite";
}

QString NikushorSite::getDescription()
{
	return "NikushorSite";
}

#include "webobjects/webpage/webpage.h"
#include "webobjects/webpage/pageobject/label/label.h"
#include "webobjects/webpage/pageobject/checkbox/checkbox.h"

QByteArray NikushorSite::invoke( QByteArray request )
{
	WebPage webPage;
	webPage.setTitle( "nicu's personal page" );

	CheckBox checkBox;
	checkBox.setText( "test check-box" );
	webPage.addObject( &checkBox );

	Label label;
	label.setText( "welcome" );
	webPage.addObject( &label );

	return webPage.toHtml().toAscii().data();
}
