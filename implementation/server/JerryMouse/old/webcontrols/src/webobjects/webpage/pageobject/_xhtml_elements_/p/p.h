#ifndef p_h
#define p_h

#include "../xhtmlelement.h"

namespace XHtmlElements
{
	/*****************************************************
	*
	*	Define the "p" element from XHTML
	*
	******************************************************/
	class P : public XHtmlElement 
	{
	public:
		P();
		~P();
	
	public:
		QString toHtml();

	};
};

#endif
