
#include "label.h"

XHtmlElements::Label::Label() :
	XHtmlElement()
{
	setType( "LABEL" );
}

XHtmlElements::Label::~Label()
{}

QString XHtmlElements::Label::toHtml()
{
	QString asHtml = "<label";
	
	asHtml+= getBasicAttributes();
	
	if( false == _for.isEmpty() )
		asHtml+=QString(" for=\"%1\" ").arg( _for );
	
	asHtml+=">"
	asHtml += getContainedObjectsAsHTML();

	asHtml += "</label>"; 

	return asHtml;
}

bool XHtmlElements::Label::getFor(){ return _for; }
void XHtmlElements::Label::setFor( QString forr ){ _for = forr; }
