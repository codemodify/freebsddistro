#ifndef img_h
#define img_h

#include "../xhtmlelement.h"

namespace XHtmlElements
{
	/*****************************************************
	*
	*	Define the "img" tag from XHTML
	*
	******************************************************/
	class Img : public XHtmlElement 
	{
	public:
		Img();
		~Img();
	
	public:
		QString toHtml();
	
	// XHTML attributes valid for this element
	private:
		QString _alt;
		QString _src;
	public:
		QString getAlt();
		QString getSrc();

		void setAlt( QString alt );
		void setSrc( QString src );

	};
};

#endif
