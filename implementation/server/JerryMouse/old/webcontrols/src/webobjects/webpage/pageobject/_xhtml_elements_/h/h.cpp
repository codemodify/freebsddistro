
#include "h.h"

XHtmlElements::H::H() :
	XHtmlElement()
{
	setType( "H" );
}

XHtmlElements::H::~H()
{}

QString XHtmlElements::H::toHtml()
{
	QString asHtml = QString("<h%1").arg(getHeaderSize());

	if( false == getBasicAttributes().isEmpty() )
		asHtml+= getBasicAttributes();
	
	asHtml = QString("%1>%2</h%3>").arg(asHtml).arg(getContentAsHtml()).arg(getHeaderSize());
	
	return asHtml;
}

QString XHtmlElements::Body::getHeaderSize(){ return _headerSize; }
void XHtmlElements::Body::seteaderSize( QString headerSize ){ _headerSize = headerSize; }
