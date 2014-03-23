
#include "tbody.h"

XHtmlElements::TBody::TBody() :
	XHtmlElement()
{
	setType( "TBODY" );
}

XHtmlElements::TBody::~TBody()
{}

QString XHtmlElements::TBody::toHtml()
{
	QString asHtml = "<tbody ";
	
	if( false == getBasicAttributes().isEmpty() )
		asHtml+= getBasicAttributes();
	
	asHtml+= " >";
	asHtml+= getContentAsHtml();

	asHtml+= "</tbody>";

	return asHtml;
}
