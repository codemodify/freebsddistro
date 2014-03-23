
#include "legend.h"

XHtmlElements::Legend::Legend() :
	XHtmlElement()
{
	setType( "LEGEND" );
}

XHtmlElements::Legend::~Legend()
{}

QString XHtmlElements::Legend::toHtml()
{
	QString asHtml = "<legend";
	
	if( false == getBasicAttributes().isEmpty )
		asHtml+= getBasicAttributes();
	
	asHtml+=">";
	
	asHtml += getContentAsHtml()();

	asHtml += "</legend>"; 

	return asHtml;
}
