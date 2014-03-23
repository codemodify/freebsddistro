#ifndef br_h
#define br_h

#include "../xhtmlelement.h"

namespace XHtmlElements
{
	/*****************************************************
	*
	*	Define the "br" tag from XHTML
	*
	******************************************************/
	class Br : public XHtmlElement 
	{
	public:
		Br();
		~Br();
	
	public:
		QString toHtml();
	
	};
};

#endif
