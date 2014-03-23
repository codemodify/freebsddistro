#ifndef select_h
#define select_h

#include "../xhtmlelement.h"

namespace XHtmlElements
{ 
	/*****************************************************
	*
	*	Define the "select" tag from XHTML
	*
	******************************************************/
	class Select : public XHtmlElement 
	{
	public:
		Select();
		~Select();
	
	public:
		QString toHtml();
		
	// XHTML attributes valid for this element
	private:
		bool _disabled;
		bool _multiple;
		QString _name;
		
		QString _onFocus;
		QString _onBlur;
		QString _onChange;
		
	public:
		bool getDisabled();
		bool getMultiple();
		bool getName();
		QString getOnFocus();
		QString getOnBlur();
		QString getOnChange();

		void setDisabled( bool disabled );
		void setMultiple( bool mutiple );
		void setName( QString name );
		void setOnFocus( QString onFocus );
		void setOnBlur( QString onBlur );
		void setOnChange( QString onChange );

	};
};

#endif
