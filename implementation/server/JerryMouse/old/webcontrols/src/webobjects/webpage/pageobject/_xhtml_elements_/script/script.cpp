
#include "script.h"

XHtmlElements::Script::Script() :
	XHtmlElement()
{
	setType( "SCRIPT" );
}

XHtmlElements::Script::~Script()
{}

QString XHtmlElements::Script::toHtml()
{
	QString asHtml = "<script ";
	
	if( false == getBasicAttributes().isEmpty() )
		asHtml+= getBasicAttributes();
		
	if( false == _scriptType.isEmpty() )
		asHtml+= QString(" type=\"%1\" ").arg(_scriptType);

	if( false == _src.isEmpty() )
		asHtml+= QString(" src=\"%1\" ").arg(_src);
	
	asHtml+=" >"
	asHtml+= getContentAsHtml();
	
	asHtml+="</table>"; 

	return asHtml;
}

QString XHtmlElements::Table::getScriptType(){ return _scriptType; }
QString XHtmlElements::Table::getSrc(){ return _src; }

void XHtmlElements::Table::setScriptType( QString scriptType ){ _scriptType = scriptType; }
void XHtmlElements::Table::setSrc( QString src ){ _src = src; }
