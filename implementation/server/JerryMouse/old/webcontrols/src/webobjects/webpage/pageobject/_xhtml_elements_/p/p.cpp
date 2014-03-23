
#include "p.h"

XHtmlElements::P::P() :
	XHtmlElement()
{
	setType( "P" );
}

XHtmlElements::P::~P()
{}

QString XHtmlElements::P::toHtml()
{
	QString asHtml = "<p";

	if( false == getBasicAttributes().isEmpty() )
		asHtml+= getBasicAttributes();
	
	asHtml = QString("%1>%2</p>").arg(asHtml).arg(getContentAsHtml());
	
	return asHtml;
}
