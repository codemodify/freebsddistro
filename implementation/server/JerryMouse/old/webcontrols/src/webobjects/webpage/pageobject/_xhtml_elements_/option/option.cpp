
#include "option.h"

XHtmlElements::Option::Option() :
	XHtmlElement()
{
	setType( "OPTION" );
}

XHtmlElements::Option::~Option()
{}

QString XHtmlElements::Option::toHtml()
{
	QString asHtml = "<option";
	
	asHtml+= getBasicAttributes();
	
	if( true == _disabled )
		asHtml+=QString( " disabled=\"disabled\" " );

	if( false == _label.isEmpty() )
		asHtml+=QString(" label=\"%1\" ").arg( _label );

	if( true == _selected )
		asHtml+=QString( " selected=\"selected\" " );

	if( false == _value.isEmpty() )
		asHtml+=QString(" value=\"%1\" ").arg( _value );
	
	asHtml+=">"
	asHtml += getContainedObjectsAsHTML();

	asHtml += "</option>"; 

	return asHtml;
}

bool XHtmlElements::Option::getDisabled(){ return _disabled; }
QString XHtmlElements::Option::getLabel(){ return _label; }
bool XHtmlElements::Option::getSelected(){ return _selected; }
QString XHtmlElements::Option::getValue(){ return _value; }

void XHtmlElements::Option::setDisabled( bool disabled ){ _disabled = disabled; }
void XHtmlElements::Option::setLabel( QString label ){ _label = label; }
void XHtmlElements::Option::setSelected( bool disabled ){ _disabled = disabled; }
void XHtmlElements::Option::setValue( QString value ){ _value = value; }
