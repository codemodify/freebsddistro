
#include "check.h"

XHtmlElements::Check::Check() :
	Input()
{
	setType( "INPUT-CHECKBOX" );
	setInputType( "checkbox" );
}

XHtmlElements::Check::~Check()
{}

QString XHtmlElements::Check::toHtml()
{
	QString asHtml = QString("<input %1 ").arg( getBasicAttributes() );
	
	if( true == _checked )
		basicAttributes += QString(" checked=\"checked\" ");
		
	asHtml+="/>"

	return asHtml;
}

bool HtmlElements::Check::getChecked(){ return _checked; }
void HtmlElements::Check::setChecked( bool checked ){ _checked = checked; }
