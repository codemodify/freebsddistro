
#include "param.h"

XHtmlElements::Param::Param() :
	XHtmlElement()
{
	setType( "PARAM" );
}

XHtmlElements::Param::~Param()
{}

QString XHtmlElements::Param::toHtml()
{
	QString asHtml = "<object ";
	
	if( false == getBasicAttributes().isEmpty() )
		asHtml+= getBasicAttributes();

	if( false == _name.isEmpty() )
		asHtml+= QString(" name=\"%1\" ").arg(_name);

	if( false == _mimeType.isEmpty() )
		asHtml+= QString(" type=\"%1\" ").arg(_mimeType);

	if( false == _value.isEmpty() )
		asHtml+= QString(" value=\"%1\" ").arg(_value);

	if( false == _valueType.isEmpty() )
		asHtml+= QString(" valueType=\"%1\" ").arg(_valueType);
	
	asHtml+=" >"
	asHtml+= getContentAsHtml();
	
	asHtml+="</object>"; 

	return asHtml;
}

QString XHtmlElements::Param::getName(){ return _name; }
QString XHtmlElements::Param::getMimeType(){ return _mimeType; }
QString XHtmlElements::Param::getValue(){ return _value; }
QString XHtmlElements::Param::getValueType(){ return _valueType; }

void XHtmlElements::Param::setName( QString name ){ _name = name; }
void XHtmlElements::Param::setMimeType( QString mimeType ){ _mimeType = mimeType; }
void XHtmlElements::Param::setValue( QString value ){ _value = value; }
void XHtmlElements::Param::setValueType( QString valueType ){ _valueType = valueType; }
