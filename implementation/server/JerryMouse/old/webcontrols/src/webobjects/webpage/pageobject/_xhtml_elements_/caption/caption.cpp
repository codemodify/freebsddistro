
#include "caption.h"

XHtmlElements::Caption::Caption() :
	XHtmlElement()
{
	setType( "CAPTION" );
}

XHtmlElements::Caption::~Caption()
{}

QString XHtmlElements::Caption::toHtml()
{
	QString asHtml = "<caption ";
	
	if( false == getBasicAttributes().isEmpty() )
		asHtml+= getBasicAttributes();
	
	asHtml+=" >"

	asHtml+= getContentAsHtml();
	
	asHtml+="</caption>"; 

	return asHtml;
}
