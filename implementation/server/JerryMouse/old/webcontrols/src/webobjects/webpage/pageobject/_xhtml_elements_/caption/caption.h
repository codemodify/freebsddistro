#ifndef caption_h
#define caption_h

#include "../xhtmlelement.h"

namespace XHtmlElements
{
	/*****************************************************
	*
	*	Define the "caption" from XHTML
	*
	******************************************************/
	class Caption : public XHtmlElement
	{
	public:
		Caption();
		~Caption();
	
	public:
		QString toHtml();

	};
};

#endif
