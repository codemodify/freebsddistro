#ifndef tfoot_h
#define tfoot_h

#include "../xhtmlelement.h"

namespace XHtmlElements
{
	/*****************************************************
	*
	*	Define the "tfoot" tag from XHTML
	*
	******************************************************/
	class TFoot : public XHtmlElement 
	{
	public:
		TFoot();
		~TFoot();
	
	public:
		QString toHtml();
	
	};
};

#endif
