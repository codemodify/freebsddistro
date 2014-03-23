
#include "select.h"

XHtmlElements::Select::Select() :
	XHtmlElement()
{
	setType( "SELECT" );
}

XHtmlElements::Select::~Select()
{}

QString XHtmlElements::Select::toHtml()
{
	QString asHtml = "<selct";
	
	asHtml+= getBasicAttributes();
	
	if( true == _disabled )
		asHtml+=QString( " disabled=\"disabled\" " );

	if( true == _multiple )
		asHtml+=QString( " multiple=\"multiple\" " );
	
	if( false == _name.isEmpty() )
		asHtml+=QString(" name=\"%1\" ").arg( _name );

	if( false == _onFocus.isEmpty() )
		asHtml+=QString(" onFocus=\"%1\" ").arg( _onFocus );

	if( false == _onBlur.isEmpty() )
		asHtml+=QString(" onBlur=\"%1\" ").arg( _onBlur );

	if( false == _onChange.isEmpty() )
		asHtml+=QString(" onChange=\"%1\" ").arg( _onChange );
	
	asHtml+=">"
	asHtml += getContainedObjectsAsHTML();

	asHtml += "</select>"; 

	return asHtml;
}

bool XHtmlElements::Select::getDisabled(){ return _dsabled; }
bool XHtmlElements::Select::getMultiple(){ return _multiple; }
QString XHtmlElements::Select::getName(){ return _name; }
QString XHtmlElements::Select::getOnFocus(){ return _onFocus; }
QString XHtmlElements::Select::getOnBlur(){ return _onBlur; }
QString XHtmlElements::Select::getOnChange(){ return _onChange; }

void XHtmlElements::Select::setDisabled( bool disabled ){ _disabled = disabled; }
void XHtmlElements::Select::setMultiple( bool mutiple ){ _multiple = multiple; }
void XHtmlElements::Select::setName( QString name ){ _name = name; }
void XHtmlElements::Select::setOnFocus( QString onFocus ){ _onFocus = onFocus; }
void XHtmlElements::Select::setOnBlur( QString onBlur ){ _onBlur = onBlur; }
void XHtmlElements::Select::setOnChange( QString onChange ){ _onChange = onChange; }
