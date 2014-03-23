#ifndef html_h
#define html_h

#include "../xhtmlelement.h"

namespace XHtmlElements
{
	/*****************************************************
	*
	*	Define the "html" from XHTML
	*
	******************************************************/
	class Html : public XHtmlElement 
	{
	public:
		Html();
		~Html();
	
	public:
		QString toHtml();
	
	};
};

#endif
