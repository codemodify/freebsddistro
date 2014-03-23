#ifndef a_h
#define a_h

#include "../xhtmlelement.h"

namespace XHtmlElements
{
	/*****************************************************
	*
	*	Define the "a" tag from XHTML
	*
	******************************************************/
	class A : public XHtmlElement 
	{
	public:
		A();
		~A();
	
	public:
		QString toHtml();
	
	// XHTML attributes valid for this element
	private:
		QString _href;
		QString _name;
		QString _target;

	public:
		QString getHref();
		QString getName();
		QString getTarget();
		void setHref( QString href );
		void setName( QString name );
		void setTarget( QString target );
	
	};
};

#endif
