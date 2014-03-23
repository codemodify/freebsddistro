
#include "head.h"

XHtmlElements::Head::Head() :
	XHtmlElement()
{
	setType( "HEAD" );
}

XHtmlElements::Head::~Head()
{}

QString XHtmlElements::Head::toHtml()
{
	QString asHtml = "<head ";
	
	if( false == getBasicAttributes().isEmpty() )
		asHtml+= getBasicAttributes();

	asHtml+= " >";
	asHtml+= getContentAsHtml();
	
	asHtml+= "</head>";

	return asHtml;
}
