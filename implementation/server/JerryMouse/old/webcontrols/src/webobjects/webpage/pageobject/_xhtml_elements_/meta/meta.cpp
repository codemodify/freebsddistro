
#include "meta.h"

XHtmlElements::Meta::MetaInfo()
{
	setType( "META" );
}

XHtmlElements::Meta::~MetaInfo()
{}

QSttring XHtmlElements::Meta::toHtml()
{
	QString asHtml = "<meta ";
	
	if( false == getBasicAttributes().isEmpty() )
		asHtml+= getBasicAttributes();
		
	if( false == _content.isEmpty() )
		asHtml+= QString(" content=\"%1\" ").arg( _content );

	if( false == _httpEquiv.isEmpty() )
		asHtml+= QString(" http-equiv=\"%1\" ").arg( _httpEquiv );

	if( false == _name.isEmpty() )
		asHtml+= QString(" name=\"%1\" ").arg( _name );
		
	if( false == _scheme.isEmpty() )
		asHtml+= QString(" scheme=\"%1\" ").arg( _scheme );
		
	asHtml+= " />";

	return asHtml;	 
}

QString XHtmlElements::Meta::getContent(){ return _content; }
QString XHtmlElements::Meta::getHttpEquiv(){ return _httpEquiv; }
QString XHtmlElements::Meta::getName(){ return _name; }
QString XHtmlElements::Meta::getScheme(){ return _scheme; }

bool XHtmlElements::Meta::getContent( QString content ){ _content = content; }
bool XHtmlElements::Meta::getHttpEquiv( QString httpEquiv ){ _httpEquiv = httpEquiv; }
bool XHtmlElements::Meta::getName( QString name ){ _name = name; }
bool XHtmlElements::Meta::getScheme( QString scheme ){ _scheme = scheme; }
