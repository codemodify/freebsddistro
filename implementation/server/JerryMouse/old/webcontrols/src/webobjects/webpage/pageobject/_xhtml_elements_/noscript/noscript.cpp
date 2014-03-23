
#include "noscript.h"

XHtmlElements::NoScript::NoScript() :
	XHtmlElement()
{
	setType( "NOSCRIPT" );
}

XHtmlElements::NoScript::~NoScript()
{}

QString XHtmlElements::NoScript::toHtml()
{
	QString asHtml = "<noscript ";
	
	if( false == getBasicAttributes().isEmpty() )
		asHtml+= getBasicAttributes();
		
	asHtml+=" >"
	asHtml+= getContentAsHtml();
	
	asHtml+="</noscript>"; 

	return asHtml;
}
