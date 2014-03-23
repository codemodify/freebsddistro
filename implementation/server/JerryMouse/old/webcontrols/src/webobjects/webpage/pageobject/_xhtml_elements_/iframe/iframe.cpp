
#include "iframe.h"

XHtmlElements::IFrame::IFrame() :
	XHtmlElement()
{
	setType( "IFRAME" );
}

XHtmlElements::IFrame::~IFrame()
{}

QString XHtmlElements::IFrame::toHtml()
{
	QString asHtml = "<iframe";
	
	if( false == getBasicAttributes().isEmpty() )
		asHtml+=getBasicAttributes();
		
	if( true == _frameBorderVisible )
		asHtml+= QString(" frameborder=\"1\" ");
		
	if( false == _height.isEmpty() )
		asHtml+= QString(" height=\"%1\" ").arg( _height );

	if( false == _marginHeight.isEmpty() )
		asHtml+= QString(" marginheight=\"%1\" ").arg( _marginHeight );

	if( false == _marginWidth.isEmpty() )
		asHtml+= QString(" marginWidth=\"%1\" ").arg( _marginWidth );

	if( false == _name.isEmpty() )
		asHtml+= QString(" name=\"%1\" ").arg( _name );

	if( false == _scrolling.isEmpty() )
		asHtml+= QString(" scrolling=\"%1\" ").arg( _scrolling );

	if( false == _src.isEmpty() )
		asHtml+= QString(" src=\"%1\" ").arg( _src );

	if( false == _width.isEmpty() )
		asHtml+= QString(" width=\"%1\" ").arg( _width );

	asHtml+=">";
	
	asHtml+= getContentAsHtml();
	
	asHtml+= "</iframe>";

	return asHtml;
}

bool XHtmlElements::IFrame::getFrameBorderVisible(){ return _frameBorderVisible; }
QString XHtmlElements::IFrame::getHeight(){ return _height; }
QString XHtmlElements::IFrame::getMarginHeight(){ return _marginHeight; }
QString XHtmlElements::IFrame::getMarginWidth(){ return _marginWidth; }
QString XHtmlElements::IFrame::getName(){ return _name; }
QString XHtmlElements::IFrame::getScrolling(){ return _scrolling; }
QString XHtmlElements::IFrame::getSrc(){ return _src; }
QString XHtmlElements::IFrame::getWidth(){ return _width; }
void XHtmlElements::IFrame::setFrameBorderVisible( bool frameBorderVisible ){ _frameBorderVisible = frameBorderVisible; }
void XHtmlElements::IFrame::setHeight( QString height ){ _height = height; }
void XHtmlElements::IFrame::setMarginHeight( QString marginHeight ){ _marginHeight = marginHeight; }
void XHtmlElements::IFrame::setMarginWidth( QString marginWidth ){ _marginWidth = marginWidth; }
void XHtmlElements::IFrame::setName( QString name ){ _name = name; }
void XHtmlElements::IFrame::setScrolling( QString scrolling ){ _scrolling = scrolling; }
void XHtmlElements::IFrame::setSrc( QString src ){ _src = src; }
void XHtmlElements::IFrame::setWidth( QString width ){ _width = width; }
