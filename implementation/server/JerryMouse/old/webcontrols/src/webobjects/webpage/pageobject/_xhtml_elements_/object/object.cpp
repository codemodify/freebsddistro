
#include "object.h"

XHtmlElements::Object::Object() :
	XHtmlElement()
{
	setType( "OBJECT" );
}

XHtmlElements::Object::~Object()
{}

QString XHtmlElements::Object::toHtml()
{
	QString asHtml = "<object ";
	
	if( false == getBasicAttributes().isEmpty() )
		asHtml+= getBasicAttributes();
		
	if( false == _archive.isEmpty() )
		asHtml+= QString(" archive=\"%1\" ").arg(_archive);

	if( false == _classid.isEmpty() )
		asHtml+= QString(" classid=\"%1\" ").arg(_classid);

	if( false == _codebase.isEmpty() )
		asHtml+= QString(" codebase=\"%1\" ").arg(_codebase);

	if( false == _codetype.isEmpty() )
		asHtml+= QString(" codetype=\"%1\" ").arg(_codetype);

	if( false == _data.isEmpty() )
		asHtml+= QString(" data=\"%1\" ").arg(_data);

	if( true == _declare )
		asHtml+= QString(" declare=\"declare\" ");

	if( false == _height.isEmpty() )
		asHtml+= QString(" height=\"%1\" ").arg(_height);

	if( false == _name.isEmpty() )
		asHtml+= QString(" name=\"%1\" ").arg(_name);

	if( false == _standby.isEmpty() )
		asHtml+= QString(" standby=\"%1\" ").arg(_standby);

	if( false == _objectType.isEmpty() )
		asHtml+= QString(" type=\"%1\" ").arg(_objectType);

	if( false == _usemap.isEmpty() )
		asHtml+= QString(" usemap=\"%1\" ").arg(_usemap);

	if( false == _width.isEmpty() )
		asHtml+= QString(" width=\"%1\" ").arg(_width);
	
	asHtml+=" >"
	asHtml+= getContentAsHtml();
	
	asHtml+="</object>"; 

	return asHtml;
}

QString XHtmlElements::Object::getArchive(){ return _archive; }
QString XHtmlElements::Object::getClassid(){ return _classid; }
QStirng XHtmlElements::Object::getCodebase(){ return _codebase; }
QString XHtmlElements::Object::getCodetype(){ return _codetype; }
QString XHtmlElements::Object::getData(){ return _data; }
bool    XHtmlElements::Object::getDeclare(){ return _declare; }
QString XHtmlElements::Object::getHeight(){ return _height; }
QString XHtmlElements::Object::getName(){ return _name; }
QString XHtmlElements::Object::getStandby(){ return _standby; }
QString XHtmlElements::Object::getObjectType(){ return _type; }
QString XHtmlElements::Object::getUsemap(){ return _usemap; }
QString XHtmlElements::Object::getWidth(){ return _width; }

void XHtmlElements::Object::setArchive( QString archive ){ _archive = archive; }
void XHtmlElements::Object::setClassid( QString classid ){ _classid = classid; }
void XHtmlElements::Object::setCodebase( QString codebase ){ _codebase = codebase; }
void XHtmlElements::Object::setCodetype( QString codetype ){ _codetype = codetype; }
void XHtmlElements::Object::setData( QString data ){ _data = data; }
void XHtmlElements::Object::setDeclare( bool declare ){ _declare = declare; }
void XHtmlElements::Object::setHeight( QString height ){ _height = height; }
void XHtmlElements::Object::setName( QString name ){ _name = name; }
void XHtmlElements::Object::setStandby( QString standby ){ _standby = standby; }
void XHtmlElements::Object::getObjectType( QString type ){ _type = type; }
void XHtmlElements::Object::setUsemap( QString usemap ){ _usemap = usemap; }
void XHtmlElements::Object::setWidth( QString width ){ _width = width; }
