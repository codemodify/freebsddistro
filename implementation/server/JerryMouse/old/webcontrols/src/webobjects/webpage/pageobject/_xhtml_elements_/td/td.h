#ifndef td_h
#define td_h

#include "../xhtmlelement.h"

namespace XHtmlElements
{
	/*****************************************************
	*
	*	Define the "td" tag from XHTML
	*
	******************************************************/
	class Td : public XHtmlElement 
	{
	public:
		Td();
		~Td();
	
	public:
		QString toHtml();
	
	};
};

#endif
