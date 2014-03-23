
#include "br.h"

XHtmlElements::Br::Br() :
	XHtmlElement()
{
	setType( "BR" );
}

XHtmlElements::Br::~Br()
{}

QString XHtmlElements::Br::toHtml()
{
	QString asHtml = QString("<br%1/>").arg(getBasicAttributes());
	
	return asHtml;
}
