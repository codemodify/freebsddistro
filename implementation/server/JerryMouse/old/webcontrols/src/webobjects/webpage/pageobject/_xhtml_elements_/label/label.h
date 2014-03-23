#ifndef label_h
#define label_h

#include "../xhtmlelement.h"

namespace XHtmlElements
{ 
	/*****************************************************
	*
	*	Define the "label" tag from XHTML
	*
	******************************************************/
	class Label : public XHtmlElement 
	{
	public:
		Label();
		~Label();
	
	public:
		QString toHtml();
		
	// XHTML attributes valid for this element
	private:
		QString _for;
	public:
		bool getFor();
		void setFor( QString forr );

	};
};

#endif
