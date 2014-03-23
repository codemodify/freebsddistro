#ifndef optgroup_h
#define optgroup_h

#include "../xhtmlelement.h"

namespace XHtmlElements
{ 
	/*****************************************************
	*
	*	Define the "optgroup" tag from XHTML
	*
	******************************************************/
	class Optgroup : public XHtmlElement 
	{
	public:
		Optgroup();
		~Optgroup();
	
	public:
		QString toHtml();
		
	// XHTML attributes valid for this element
	private:
		QString _label;
		bool _disabled;
		
	public:
		QString getLabel();
		bool getDisabled();

		void setLabel( QString label );
		void setDisabled( bool disabled );

	};
};

#endif
