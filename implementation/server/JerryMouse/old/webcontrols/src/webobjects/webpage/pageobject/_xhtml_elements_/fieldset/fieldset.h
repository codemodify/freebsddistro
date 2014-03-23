#ifndef fieldset_h
#define fieldset_h

#include "../xhtmlelement.h"

namespace XHtmlElements
{
	/*****************************************************
	*
	*	Define the "fieldset" tag from XHTML
	*
	******************************************************/
	class Fieldset : public XHtmlElement 
	{
	public:
		Fieldset();
		~Fieldset();
	
	public:
		QString toHtml();
	
	};
};

#endif
