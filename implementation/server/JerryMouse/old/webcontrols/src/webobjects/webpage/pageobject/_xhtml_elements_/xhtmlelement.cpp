
#include <QtCore/QString>

#include "xhtmlelement.h"

XHtmlElements::XHtmlElement::XHtmlElements()
{
	setType( "XHtmlElement" );
}

XHtmlElements::XHtmlElement::~XHtmlElement()
{}

QString XHtmlElements::XHtmlElement::getContentAsHtml(){ return _content; }
void    XHtmlElements::XHtmlElement::setContent( QString content ){ _content = content; }

QString XHtmlElements::XHtmlElement::getType(){ return _type; }
void    XHtmlElements::XHtmlElement::setType( QString type ){ _type = type; }

QString XHtmlElements::XHtmlElement::getClass(){ return _class; }
QString XHtmlElements::XHtmlElement::getID(){ return _id; }
QString XHtmlElements::XHtmlElement::getStyle(){ return _style; }
QString XHtmlElements::XHtmlElement::getTitle(){ return _title; }
void    XHtmlElements::XHtmlElement::setClass( QString cllass ){ _class = cllass; }
void    XHtmlElements::XHtmlElement::setID( QString id ){ _id = id; }
void    XHtmlElements::XHtmlElement::setStyle( QString style ){ _style = style; }
void    XHtmlElements::XHtmlElement::setTitle( QString title ){ _title = title; }

QString XHtmlElements::XHtmlElement::getDir(){ return _dir; }
QString XHtmlElements::XHtmlElement::getLang(){ return _lang; }
void    XHtmlElements::XHtmlElement::setDir( QString dir ){ _dir = dir; }
void    XHtmlElements::XHtmlElement::setLang( QString lang ){ _lang = lang; }

QString XHtmlElements::XHtmlElement::getAccessKey(){ return _accessKey; }
QString XHtmlElements::XHtmlElement::getTabIndex(){ return _tabIndex; }
void    XHtmlElements::XHtmlElement::setAccessKey( QString accessKey ){ _accessKey = accessKey; }
void    XHtmlElements::XHtmlElement::setTabIndex( QString tabIndex ){ _tabIndex = tabIndex; }

QString XHtmlElements::XHtmlElement::getOnClick(){ return _onClick; }
QString XHtmlElements::XHtmlElement::getOnDoubleClick(){ return _onDoubleClick; }
QString XHtmlElements::XHtmlElement::getOnMouseDown(){ return _onMouseDown; }
QString XHtmlElements::XHtmlElement::getOnMouseMove(){ return _onMouseMove; }
QString XHtmlElements::XHtmlElement::getOnMouseOver(){ return _onMouseOver; }
QString XHtmlElements::XHtmlElement::getOnMouseOut(){ return _onMouseOut; }
QString XHtmlElements::XHtmlElement::getOnMouseUp(){ return _onMouseUp; }
void    XHtmlElements::XHtmlElement::setOnClick( QString onClick ){ onClick = _onClick; }
void    XHtmlElements::XHtmlElement::setOnDoubleClick( QString onDoubleClick ){ _onDoubleClick = onDoubleClick; }
void    XHtmlElements::XHtmlElement::setOnMouseDown( QString onMouseDown ){ _onMouseDown = onMouseDown; }
void    XHtmlElements::XHtmlElement::setOnMouseMove( QString onMouseMove ){ _onMouseMove = onMouseMove; }
void    XHtmlElements::XHtmlElement::setOnMouseOver( QString onMouseOver ){ _onMouseOver = onMouseOver; }
void    XHtmlElements::XHtmlElement::setOnMouseOut( QString onMouseOut ){ _onMouseOut = onMouseOut; }
void    XHtmlElements::XHtmlElement::setOnMouseUp( QString onMouseUp ){ _onMouseUp = onMouseUp; }

QString XHtmlElements::XHtmlElement::getOnKeyDown(){ return _onKeyDown; }
QString XHtmlElements::XHtmlElement::getOnKeyPress(){ return _onKeyPress; }
QString XHtmlElements::XHtmlElement::getOnKeyUp(){ return _onKeyUp; }
void    XHtmlElements::XHtmlElement::setOnKeyDown( QString onKeyDown ){ _onKeyDown = _onKeyDown; }
void    XHtmlElements::XHtmlElement::setOnKeyPress( QString onKeyPress ){ _onKeyPress = onKeyPress; }
void    XHtmlElements::XHtmlElement::setOnKeyUp( QString onKeyUp ){ _onKeyUp = onKeyUp; }

QString XHtmlElements::XHtmlElement::getBasicAttributes()
{
	QString basicAttributes;
	
	if( false == _class.isEmpty() )
		basicAttributes += QString(" class=\"%1\" ").arg( _class );
		
	if( false == _id.isEmpty() )
		basicAttributes += QString(" id=\"%1\" ").arg( _id );
		
	if( false == _style.isEmpty() )
		basicAttributes += QString(" style=\"%1\" ").arg( _style );
		
	if( false == _title.isEmpty() )
		basicAttributes += QString(" title=\"%1\" ").arg( _title );
		
	if( false == _dir.isEmpty() )
		basicAttributes += QString(" dir=\"%1\" ").arg( _dir );
		
	if( false == _lang.isEmpty() )
		basicAttributes += QString(" lang=\"%1\" ").arg( _lang );
		
	if( false == _accessKey.isEmpty() )
		basicAttributes += QString(" accessKey=\"%1\" ").arg( _accessKey );
		
	if( false == _tabIndex.isEmpty() )
		basicAttributes += QString(" tabIndex=\"%1\" ").arg( _tabIndex );
		
	if( false == _onClick.isEmpty() )
		basicAttributes += QString(" onClick=\"%1\" ").arg( _onClick );
		
	if( false == _onDoubleClick.isEmpty() )
		basicAttributes += QString(" onDoubleClick=\"%1\" ").arg( _onDoubleClick );
		
	if( false == _onMouseDown.isEmpty() )
		basicAttributes += QString(" onMouseDown=\"%1\" ").arg( _onMouseDown );
		
	if( false == _onMouseMove.isEmpty() )
		basicAttributes += QString(" onMouseMove=\"%1\" ").arg( _onMouseMove );
		
	if( false == _onMouseOver.isEmpty() )
		basicAttributes += QString(" onMouseOver=\"%1\" ").arg( _onMouseOver );
		
	if( false == _onMouseOut.isEmpty() )
		basicAttributes += QString(" onMouseOut=\"%1\" ").arg( _onMouseOut );
		
	if( false == _onMouseUp.isEmpty() )
		basicAttributes += QString(" onMouseUp=\"%1\" ").arg( _onMouseUp );
		
	if( false == _onKeyDown.isEmpty() )
		basicAttributes += QString(" onKeyDown=\"%1\" ").arg( _onKeyDown );
		
	if( false == _onKeyPress.isEmpty() )
		basicAttributes += QString(" _onKeyPress=\"%1\" ").arg( _onKeyPress );

	if( false == _onKeyUp.isEmpty() )
		basicAttributes += QString(" _onKeyUp=\"%1\" ").arg( _onKeyUp );
		
	return basicAttributes;
}
