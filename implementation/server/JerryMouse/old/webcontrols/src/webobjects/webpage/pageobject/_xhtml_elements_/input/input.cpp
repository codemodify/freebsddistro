
#include "input.h"

XHtmlElements::Input::Input():
	XHtmlElement()
{
	setType( "INPUT" );
}

XHtmlElements::Input::~Input()
{}

bool XHtmlElements::Input::getDisabled(){ return _disabled; }
QString XHtmlElements::Input::getName(){ return _name; }
QString XHtmlElements::Input::getInputType(){ return _inputType; }
QString XHtmlElements::Input::getValue(){ return _value; }
QString XHtmlElements::Input::getOnFocus(){ return _onFocus; }
QString XHtmlElements::Input::getOnBlur(){ return _onBlur; }
QString XHtmlElements::Input::getOnChange(){ return _onChange; }
void XHtmlElements::Input::setDisabled( bool disabled ){ _disabled = disabled; }
void XHtmlElements::Input::getName( QString name ){ _name = name; }
void XHtmlElements::Input::setInputType( QString inputType ){ _inputType = inputType; }
void XHtmlElements::Input::setValue( QString value ){ _value = value; }
void XHtmlElements::Input::setOnFocus( QString onFocus ){ _onFocus = onFocus; }
void XHtmlElements::Input::setOnBlur( QString onBlur ){ _onBlur = onBlur; }
void XHtmlElements::Input::setOnChange( QString onChange ){ _onChange = onChange; }

QString XHtmlElements::Input::getBasicAttributes()
{
	QString basicAttributes = XHtmlElement::getBasicAttributes();
	
	if( true == _disabled )
		basicAttributes += QString(" disabled=\"disabled\" ");
		
	if( false == _name.isEmpty() )
		basicAttributes += QString(" name=\"%1\" ").arg( _name );
		
	if( false == _inputType.isEmpty() )
		basicAttributes += QString(" type=\"%1\" ").arg( _inputType );
		
	if( false == _value.isEmpty() )
		basicAttributes += QString(" value=\"%1\" ").arg( _value );
		
	if( false == _onFocus.isEmpty() )
		basicAttributes += QString(" onFocus=\"%1\" ").arg( _onFocus );
		
	if( false == _onBlur.isEmpty() )
		basicAttributes += QString(" onBlur=\"%1\" ").arg( _onBlur );
		
	if( false == _onChange.isEmpty() )
		basicAttributes += QString(" onChange=\"%1\" ").arg( _onChange );
		
	return basicAttributes;
}

QString XHtmlElements::Input::toHtml()
{
	QString asHtml = QString("<input %1 />").arg( getBasicAttributes() );
	
	return asHtml;  
}