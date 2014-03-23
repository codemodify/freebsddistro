
#include "hr.h"

XHtmlElements::Hr::Hr() :
	XHtmlElement()
{
	setType( "hr" );
}

XHtmlElements::Hr::~Hr()
{}

QString XHtmlElements::Hr::toHtml()
{
	QString asHtml = QString("<hr%1/>").arg(getBasicAttributes());

	return asHtml;
}
