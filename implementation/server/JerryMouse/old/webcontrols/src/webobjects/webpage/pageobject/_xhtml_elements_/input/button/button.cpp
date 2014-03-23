
#include "button.h"

XHtmlElements::Button::Button() :
	Input()
{
	setType( "INPUT-BUTTON" );
	setInputType( "button" )
}

XHtmlElements::Button::~Button()
{}
