#ifndef iframe_h
#define iframe_h

#include "../xhtmlelement.h"

namespace XHtmlElements
{
	/*****************************************************
	*
	*	Define the "iframe" tag from XHTML
	*
	******************************************************/
	class IFrame : public XHtmlElement 
	{
	public:
		IFrame();
		~IFrame();
	
	public:
		QString toHtml();

	// XHTML attributes valid for this element
	private:
		bool _frameBorderVisible;
		QString _height;
		QString _marginHeight;
		QString _marginWidth;
		QString _name;
		QString _scrolling;
		QString _src;
		QString _width;
	
	public:
		bool getFrameBorderVisible();
		QString getHeight();
		QString getMarginHeight();
		QString getMarginWidth();
		QString getName();
		QString getScrolling();
		QString getSrc();
		QString getWidth();
		void setFrameBorderVisible( bool frameBorderVisible );
		void setHeight( QString height );
		void setMarginHeight( QString marginHeight );
		void setMarginWidth( QString marginWidth );
		void setName( QString name );
		void setScrolling( QString scrolling );
		void setSrc( QString src );
		void setWidth( QString width );

	};
};

#endif
