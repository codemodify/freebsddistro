
#include "doctype.h"

XHtmlElements::DocType::DocType()
{}

XHtmlElements::DocType::~DocType()
{}

QString XHtmlElements::DocType::toHtml()
{
	if( Strict == _docType )
		return QString( "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">" );

	if( Transitional == _docType )
		return QString( "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">" );

	if( Frameset == _docType )
		return QString( "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Frameset//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd\">" );

	return "";
}

XHtmlElements::DocType::DocumentType XHtmlElements::DocType::getDocType(){ return _docType; }
void XHtmlElements::DocType::getDocType( XHtmlElements::DocType::DocumentType documentType ){ _docType = documentType; }
