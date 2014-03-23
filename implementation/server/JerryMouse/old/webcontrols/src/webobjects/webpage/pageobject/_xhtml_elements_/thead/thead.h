#ifndef thead_h
#define thead_h

#include "../xhtmlelement.h"

namespace XHtmlElements
{
	/*****************************************************
	*
	*	Define the "thead" tag from XHTML
	*
	******************************************************/
	class THead : public XHtmlElement 
	{
	public:
		THead();
		~THead();
	
	public:
		QString toHtml();
	
	};
};
#endif
