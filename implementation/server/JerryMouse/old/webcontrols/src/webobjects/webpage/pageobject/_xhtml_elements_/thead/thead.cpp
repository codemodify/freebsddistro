
#include "thead.h"

XHtmlElements::THead::THead() :
	XHtmlElement()
{
	setType( "thead" );
}

XHtmlElements::THead::~THead()
{}

QString XHtmlElements::THead::toHtml()
{
	QString asHtml = "<thead ";
	
	if( false == getBasicAttributes().isEmpty() )
		asHtml+= getBasicAttributes();
	
	asHtml+= " >";
	asHtml+= getContentAsHtml();

	asHtml += "</thead>"; 

	return asHtml;
}
