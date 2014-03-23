
#include "tr.h"

XHtmlElements::Tr::Tr() :
	HtmlObject()
{
	setType( "TR" );
}

XHtmlElements::Tr::~Tr()
{}

QString XHtmlElements::Tr::toHtml()
{
	QString asHtml = "<tr ";
	
	if( false == getBasicAttributes().isEmpty() )
		asHtml+= getBasicAttributes(); 
	
	asHtml+=" >";
	
	asHtml += getContentAsHtml();

	asHtml += "</tr>"; 

	return asHtml;
}
