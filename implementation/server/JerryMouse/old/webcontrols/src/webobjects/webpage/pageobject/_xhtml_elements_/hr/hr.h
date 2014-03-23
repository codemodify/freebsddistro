#ifndef hr_h
#define hr_h

#include "../xhtmlelement.h"

namespace XHtmlElements
{
	/*****************************************************
	*
	*	Define the "hr" tag from XHTML
	*
	******************************************************/
	class Hr : public XHtmlElement 
	{
	public:
		Hr();
		~Hr();
	
	public:
		QString toHtml();

	};
};

#endif
