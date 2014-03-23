
#include "optgroup.h"

XHtmlElements::Optgroup::Optgroup() :
	XHtmlElement()
{
	setType( "OPTGROUP" );
}

XHtmlElements::Optgroup::~Optgroup()
{}

QString XHtmlElements::Optgroup::toHtml()
{
	QString asHtml = "<optgroup";
	
	asHtml+= getBasicAttributes();
	
	if( true == _disabled )
		asHtml+=QString( " disabled=\"disabled\" " );

	if( false == _label.isEmpty() )
		asHtml+=QString(" label=\"%1\" ").arg( _label );
	
	asHtml+=">"
	asHtml += getContainedObjectsAsHTML();

	asHtml += "</optgroup>"; 

	return asHtml;
}

bool XHtmlElements::Select::getDisabled(){ return _dsabled; }
QString XHtmlElements::Select::getOnChange(){ return _onChange; }

void XHtmlElements::Select::setDisabled( bool disabled ){ _disabled = disabled; }
void XHtmlElements::Select::setOnChange( QString onChange ){ _onChange = onChange; }
