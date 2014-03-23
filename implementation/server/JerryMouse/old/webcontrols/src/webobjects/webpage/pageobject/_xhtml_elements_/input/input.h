#ifndef input_h
#define input_h

#include "../xhtmlelement.h"

namespace XHtmlElements
{
	/*****************************************************
	*
	*	Define the "input" from XHTML
	*
	******************************************************/
	class Input : public XHtmlElement
	{
	public:
		Input();
		virtual ~Input();
		
		virtual QString toHtml();
		
		QString getBasicAttributes();

	// XHTML attributes valid for this element
	private:
		bool _disabled;
		QString _name;
		QString _inputType;
		QString _value;
		QString _onFocus;
		QString _onBlur;
		QString _onChange;
		
	public:
		bool getDisabled();
		QString getName();
		QString getInputType();
		QString getValue();
		QString getOnFocus();
		QString getOnBlur();
		QString getOnChange();
		void setDisabled( bool disabled );
		void getName( QString name );
		void setInputType( QString inputType );
		void setValue( QString value );
		void setOnFocus( QString onFocus );
		void setOnBlur( QString onBlur );
		void setOnChange( QString onChange );

	};
}

#endif
