#ifndef noscript_h
#define noscript_h

#include "../xhtmlelement.h"

namespace XHtmlElements
{
	/*****************************************************
	*
	*	Define the "script" from XHTML
	*
	******************************************************/
	class NoScript : public XHtmlElement
	{
	public:
		NoScript();
		~NoScript();
	
	public:
		QString toHtml();
	
	};
};

#endif
