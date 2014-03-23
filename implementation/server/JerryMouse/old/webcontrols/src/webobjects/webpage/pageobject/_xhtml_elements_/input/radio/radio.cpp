
#include "radio.h"

XHtmlElements::Radio::Radio() :
	Input()
{
	setType( "INPUT-RADIO" );
	setInputType( "radio" );
}

XHtmlElements::Radio::~Radio()
{}

QString XHtmlElements::Radio::toHtml()
{
	QString asHtml = QString("<input %1 ").arg( getBasicAttributes() );
	
	if( true == _checked )
		basicAttributes += QString(" checked=\"checked\" ");
		
	asHtml+="/>"

	return asHtml;
}

bool HtmlElements::Check::getChecked(){ return _checked; }
void HtmlElements::Check::setChecked( bool checked ){ _checked = checked; }
