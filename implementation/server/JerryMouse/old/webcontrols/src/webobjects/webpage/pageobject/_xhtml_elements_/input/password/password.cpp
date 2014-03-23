
#include "password.h"

XHtmlElements::Password::Password() :
	Input()
{
	setType( "INPUT-PASSWORD" );
	setInputType( "password" );
}

XHtmlElements::Password::~Password()
{}
