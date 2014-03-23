#ifndef legend_h
#define legend_h

#include "../xhtmlelement.h"

namespace XHtmlElements
{
	/*****************************************************
	*
	*	Define the "legend" tag from XHTML
	*
	******************************************************/
	class Legend : public XHtmlElement 
	{
	public:
		Legend();
		~Legend();
	
	public:
		QString toHtml();
	
	};
};

#endif
