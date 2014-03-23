#ifndef tbody_h
#define tbody_h

#include "../xhtmlelement.h"

namespace XHtmlElements
{
	/*****************************************************
	*
	*	Define the "tbody" tag from XHTML
	*
	******************************************************/
	class TBody : public XHtmlElement 
	{
	public:
		TBody();
		~TBody();
	
	public:
		QString toHtml();
	
	};
};

#endif
