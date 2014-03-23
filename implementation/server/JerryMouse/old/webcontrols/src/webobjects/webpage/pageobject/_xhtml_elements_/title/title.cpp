
#include "title.h"

XHtmlElements::Title::Title() :
	XHtmlElement()
{
	_type = "TITLE";
}

XHtmlElements::Title::~Title()
{}

QString XHtmlElements::Title::toHtml()
{
	QString asHtml = "<title ";
	
	if( false == getBasicAttributes().isEmpty() )
		asHtml+= getBasicAttributes();
		
	asHtml+= " >";
	asHtml+= getContentAsHtml();
	
	asHtml+= "</title>";

	return asHtml;
}
