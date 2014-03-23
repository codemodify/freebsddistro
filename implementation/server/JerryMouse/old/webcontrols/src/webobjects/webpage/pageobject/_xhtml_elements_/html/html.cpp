
#include "html.h"

XHtmlElements::Html::Html() :
	XHtmlElement()
{
	setType( "HTML" );
}

XHtmlElements::Html::~Html()
{}

QString XHtmlElements::Html::toHtml()
{
	QString asHtml = "<html>";

	asHtml+= getContentAsHtml();
	
	asHtml+= "</html>";

	return asHtml;
}
