#ifndef title_h
#define title_h

#include "../xhtmlelement.h"

namespace XHtmlElements
{
	/*****************************************************
	*
	*	Define the "title tag" from XHTML
	*
	******************************************************/
	class Title : public XHtmlElement 
	{
	public:
		Title();
		~Title();
	
	public:
		QString toHtml();
	
	};
};

#endif
