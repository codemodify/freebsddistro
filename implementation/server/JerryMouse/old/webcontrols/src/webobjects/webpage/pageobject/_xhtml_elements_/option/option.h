#ifndef option_h
#define option_h

#include "../xhtmlelement.h"

namespace XHtmlElements
{ 
	/*****************************************************
	*
	*	Define the "option" tag from XHTML
	*
	******************************************************/
	class Option : public XHtmlElement 
	{
	public:
		Option();
		~Option();
	
	public:
		QString toHtml();
		
	// XHTML attributes valid for this element
	private:
		bool _disabled;
		QString _label;
		bool _selected;
		QString _value;

	public:
		bool getDisabled();
		QString getLabel();
		bool getSelected();
		QString getValue();

		void setDisabled( bool disabled );
		void setLabel( QString label );
		void setSelected( bool disabled );
		void setValue( QString value );

	};
};

#endif
