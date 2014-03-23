
#include "td.h"

XHtmlElements::Td::Td() :
	HtmlObject()
{
	setType( "TD" );
}

XHtmlElements::Td::~Td()
{}

QString XHtmlElements::Td::toHtml()
{
	QString asHtml = "<td ";
	
	if( false == getBasicAttributes().isEmpty() )
		asHtml+= getBasicAttributes(); 
	
	asHtml+=" >";
	
	asHtml += getContentAsHtml();

	asHtml += "</td>"; 

	return asHtml;
}
