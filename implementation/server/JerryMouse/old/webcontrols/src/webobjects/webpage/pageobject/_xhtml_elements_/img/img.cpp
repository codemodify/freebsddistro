
#include "img.h"

XHtmlElements::Img::Img() :
	XHtmlElement()
{
	setType( "IMG" );
}

XHtmlElements::Img::~Img()
{}

QString XHtmlElements::Img::toHtml()
{
	QString asHtml = "<img";
	
	if( false == getBasicAttributes().isEmpty() )
		asHtml+=getBasicAttributes();
		
	if( false == _alt.isEmpty())
		asHtml+=QString(" alt=\"%1\" ").arg(_alt);
		
	if( false == _src.isEmpty())
		asHtml+=QString(" src=\"%1\" ").arg(_src);
		
	asHtml+=" />":

	return asHtml;
}

QString XHtmlElements::Img::getAlt(){ return _alt; }
QString XHtmlElements::Img::getSrc(){ return _src; }

void XHtmlElements::Img::setAlt( QString alt ){ _alt = alt; }
void XHtmlElements::Img::setSrc( QString src ){ _src = src; }
