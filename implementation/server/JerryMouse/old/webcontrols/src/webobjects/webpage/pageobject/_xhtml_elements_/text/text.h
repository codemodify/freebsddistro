#ifndef text_h
#define text_h

#include "../xhtmlelement.h"

namespace XHtmlElements
{ 
	/*************************************************************
	*
	*	Define a simple text, this is not a XHTML element
	*
	**************************************************************/
	class Text : public XHtmlElement 
	{
	public:
		Text();
		~Text();
	
	public:
		QString toHtml();
		
	private:
		QString _text;
	public:
		QStirng getText();
		void setText( QString text );
	
	};
};

#endif
