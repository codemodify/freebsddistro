#ifndef tr_h
#define tr_h

#include "../xhtmlelement.h"

namespace XHtmlElements
{ 
	/*****************************************************
	*
	*	Define the "tr" tag from XHTML
	*
	******************************************************/
	class Tr : public XHtmlElement 
	{
	public:
		Tr();
		~Tr();
	
	public:
		QString toHtml();
	
	};
};

#endif
