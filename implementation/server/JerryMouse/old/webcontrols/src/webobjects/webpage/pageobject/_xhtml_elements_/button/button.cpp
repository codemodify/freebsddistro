
#include "button.h"

XHtmlElements::ButtonImage::ButtonImage() :
	XHtmlElement()
{
	setType( "BUTTON" );
}

XHtmlElements::ButtonImage::~ButtonImage()
{}

QString XHtmlElements::ButtonImage::toHtml()
{
	QString asHtml = "<button ";
	
	if( false == getBasicAttributes().isEmpty() )
		asHtml+= getBasicAttributes();

	if( true == _disabled )
		asHtml+= QString( " disabled=\"disabled\" " );

	if( false == _name.isEmpty() )
		asHtml+= QString( " name=\"%1\" " ).arg(_name);

	if( false == _buttonType.isEmpty() )
		asHtml+= QString( " type=\"%1\" " ).arg(_buttonType);

	if( false == _value.isEmpty() )
		asHtml+= QString( " value=\"%1\" " ).arg(_value);
	
	asHtml+=" >"
	asHtml+= getContentAsHtml();
	
	asHtml+="</button>";

	return asHtml;
}

bool XHtmlElements::ButtonImage::getDisabled(){ return _disabled; }
QString XHtmlElements::ButtonImage::getName(){ return _name; }
QString XHtmlElements::ButtonImage::getButtonType(){ return _buttonType; }
QString XHtmlElements::ButtonImage::getValue(){ return _value; }

void XHtmlElements::ButtonImage::getDisabled( bool disabled ){ _disabled = disabled; }
void XHtmlElements::ButtonImage::getName( QString name ){ _name = name; }
void XHtmlElements::ButtonImage::getButtonType( QString buttonType ){ _buttonType = buttonType; }
void XHtmlElements::ButtonImage::getValue( QString value ){ _value = value; }

