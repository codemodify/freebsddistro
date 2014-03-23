
#include "tfoot.h"

XHtmlElements::TFoot::TFoot() :
	XHtmlElement()
{
	setType( "TFOOT" );
}

XHtmlElements::TFoot::~TFoot()
{}

QString XHtmlElements::TFoot::toHtml()
{
	QString asHtml = "<tfoot ";
	
	if( false == getBasicAttributes().isEmpty() )
		asHtml+= getBasicAttributes();
	
	asHtml+= " >";
	asHtml+= getContentAsHtml();

	asHtml += "</tfoot>"; 

	return asHtml;
}
