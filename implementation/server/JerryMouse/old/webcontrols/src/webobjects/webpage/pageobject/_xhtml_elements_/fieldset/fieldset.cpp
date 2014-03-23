
#include "fieldset.h"

XHtmlElements::Fieldset::Fieldset() :
	XHtmlElement()
{
	setType( "FIELDSET" );
}

XHtmlElements::Fieldset::~Fieldset()
{}

QString XHtmlElements::Fieldset::toHtml()
{
	QString asHtml = "<fieldset";
	
	if( false == getBasicAttributes().isEmpty )
		asHtml+= getBasicAttributes();
	
	asHtml+=">";
	
	asHtml += getContentAsHtml()();

	asHtml += "</fieldset>"; 

	return asHtml;
}
