
#include "edit.h"

XHtmlElements::Edit::Edit() :
	Input()
{
	setType( "INPUT-EDIT" );
	setInputType( "text" );
}

XHtmlElements::Edit::~Edit()
{}

bool XHtmlElements::Edit::getReadOnly(){ return _readOnly; }
void XHtmlElements::Edit::setReadOnly( bool readOnly ){ _readOnly = readOnly; }

QString XHtmlElements::Edit::toHtml()
{
	QString asHtml = QString("<input %1 ").arg( getBasicAttributes() );
	
	if( true == _readOnly )
		basicAttributes += QString(" readonly=\"readonly\" ");
		
	asHtml+="/>"

	return asHtml;
}
