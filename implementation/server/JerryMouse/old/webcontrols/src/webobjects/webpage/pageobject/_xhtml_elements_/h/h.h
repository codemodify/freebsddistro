#ifndef h_h
#define h_h

#include "../xhtmlelement.h"

namespace XHtmlElements
{
	/*****************************************************
	*
	*	Define the "h" element from XHTML
	*
	******************************************************/
	class H : public XHtmlElement 
	{
	public:
		H();
		~H();
	
	public:
		QString toHtml();

	// stores what kind of H is this
	private:
		QString _headerSize; // valid values "1","2","3","4","5","6"
	public:
		QString getHeaderSize();
		void seteaderSize( QString headerSize );

	};
};

#endif
