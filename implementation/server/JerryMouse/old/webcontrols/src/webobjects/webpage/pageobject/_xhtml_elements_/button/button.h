#ifndef button_h
#define button_h

#include "../xhtmlelement.h"

namespace XHtmlElements
{
	/*****************************************************
	*
	*	Define the "caption" from XHTML
	*
	******************************************************/
	class ButtonImage : public XHtmlElement
	{
		public:
			ButtonImage();
			~ButtonImage();
		
		public:
			QString toHtml();
			
		// XHTML attributes valid for this element
		private:
			bool _disabled;
			QString _name;
			QString _buttonType;
			QString _value;
		public:
			bool getDisabled();
			QString getName();
			QString getButtonType();
			QString getValue();

			void getDisabled( bool disabled );
			void getName( QString name );
			void getButtonType( QString buttonType );
			void getValue( QString value );
			
	};
};

#endif
