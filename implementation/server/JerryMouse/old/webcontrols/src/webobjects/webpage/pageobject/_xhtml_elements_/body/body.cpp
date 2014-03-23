
#include "body.h"

XHtmlElements::Body::Body() :
	XHtmlElement()
{
	setType( "BODY" );
}

XHtmlElements::Body::~Body()
{}

QString XHtmlElements::Body::toHtml()
{
	QString asHtml = "<body";
	
	if( false == getBasicAttributes().isEmpty() )
		asHtml+=QString("%1").arg( getBasicAttributes() );
	
	if( false == _onLoad.isEmpty() )
		asHtml+=QString(" onload=\"%1\" ").arg( getOnLoad() );

	if( false == _onUnload.isEmpty() )
		asHtml+=QString(" onunload=\"%1\" ").arg( getOnUnload() );
		
	asHtml = QString("%1>%2</body>").arg(asHtml).arg(getContentAsHtml());

	return asHtml;
}

QString XHtmlElements::Body::getOnLoad(){ return _onLoad; }
QString XHtmlElements::Body::getOnUnload(){ return _onUnload; }
void XHtmlElements::Body::setOnLoad( QString onLoad ){ _onLoad = onLoad; }
void XHtmlElements::Body::setOnUnload( QString onUnload ){ _onUnload = onUnload; }
