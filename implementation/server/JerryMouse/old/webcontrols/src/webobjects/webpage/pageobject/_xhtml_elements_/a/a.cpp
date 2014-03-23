
#include "a.h"

XHtmlElements::A::A() :
	XHtmlElement()
{
	setType( "A" );
	_target= "_self";
}

XHtmlElements::A::~A()
{}

QString XHtmlElements::A::toHtml()
{
	QString asHtml = "<a href=\"%1\" target=\"%2\" %3>";
	
	asHtml = asHtml.arg( getHref() );
	asHtml = asHtml.arg( getTarget() );
	asHtml = asHtml.arg( getBasicAttributes() );

	asHtml+= getContentAsHtml()();
	asHtml+= "</a>"; 

	return asHtml;
}

QString HtmlElements::A::getHref(){ return _href; }
QString HtmlElements::A::getName(){ return _name; }
QString HtmlElements::A::getTarget(){ return _target; }

void HtmlElements::A::setHref( QString href ){ _href = href; }
void HtmlElements::A::setHref( QString name ){ _name = name; }
void HtmlElements::A::setTarget( QString target ){ _target = target; }
