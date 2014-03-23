#ifndef head_h
#define head_h

#include "../xhtmlelement.h"

namespace XHtmlElements
{
	/*****************************************************
	*
	*	Define the "head" from XHTML
	*
	******************************************************/
	class Head : public XHtmlElement
	{
	public:
		Head();
		~Head();
	
	public:
		QString toHtml();
	
	};
};

#endif
