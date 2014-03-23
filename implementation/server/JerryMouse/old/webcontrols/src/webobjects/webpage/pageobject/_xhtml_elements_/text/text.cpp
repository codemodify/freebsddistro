
#include "text.h"

XHtmlElements::Text::Text() :
	XHtmlElement()
{
	setType( "TEXT" );
}

XHtmlElements::Text::~Text()
{}

QString XHtmlElements::Text::toHtml()
{
	return _text;
}

QString XHtmlElements::Text::getText(){ return _text; }
void XHtmlElements::Text::setText( QString text ){ _text = text; }
