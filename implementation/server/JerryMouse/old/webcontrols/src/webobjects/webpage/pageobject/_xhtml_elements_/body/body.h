#ifndef body_h
#define body_h

#include "../xhtmlelement.h"

namespace XHtmlElements
{
	/*****************************************************
	*
	*	Define the "body" element from XHTML
	*
	******************************************************/
	class Body : public XHtmlElement 
	{
	public:
		Body();
		~Body();
	
	public:
		QString toHtml();

	// XHTML events that are valid only for this element
	private:
		QString _onLoad;
		QStirng _onUnload;
		
	public:
		QString getOnLoad();
		QString getOnUnload();
		void setOnLoad( QString onLoad );
		void setOnUnload( QString onUnload );
	
	};
};

#endif
