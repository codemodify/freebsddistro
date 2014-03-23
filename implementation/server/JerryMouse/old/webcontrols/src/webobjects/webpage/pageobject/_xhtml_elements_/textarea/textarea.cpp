
#include "textarea.h"

XHtmlElements::TextArea::TextArea() :
	XHtmlElement()
{
	setType( "TEXTAREA" );
}

XHtmlElements::TextArea::~TextArea()
{}

QString XHtmlElements::TextArea::toHtml()
{
	QString asHtml = "<textarea";
	
	if( false == getBasicAttributes().isEmpty() )
		asHtml+=getBasicAttributes();
		
	if( false == _cols.isEmpty() )
		asHtml+= QString(" cols=\"%1\" ").arg( _cols );

	if( false == _rows.isEmpty() )
		asHtml+= QString(" rows=\"%1\" ").arg( _rows );

	if( true == _disabled )
		asHtml+= QString(" _disabled=\"disabled\" ");

	if( false == _name.isEmpty() )
		asHtml+= QString(" name=\"%1\" ").arg( _name );

	if( true == _readonly )
		asHtml+= QString(" readonly=\"readonly\" ");

	asHtml+=">";
	
	asHtml+= getContentAsHtml();
	
	asHtml+= "</textarea>";

	return asHtml;
}

QString XHtmlElements::TextArea::getCols(){ return _cols; }
QString XHtmlElements::TextArea::getRows(){ return _rows; }
bool XHtmlElements::TextArea::getDisabled(){ return _disabled; }
QString XHtmlElements::TextArea::getName(){ return _name; }
bool XHtmlElements::TextArea::getReadonly(){ return _readonly; }

void XHtmlElements::TextArea::setCols( QString cols ){ _cols = cols; }
void XHtmlElements::TextArea::setRows( QString rows ){ _rows = rows; }
void XHtmlElements::TextArea::setDisabled( bool disabled ){ _disabled = disabled; }
void XHtmlElements::TextArea::setName( QString name ){ _name = name; }
void XHtmlElements::TextArea::setReadonly( bool readonly ){ _readonly = readonly; }
