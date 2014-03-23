#ifndef param_h
#define param_h

#include "../xhtmlelement.h"

namespace XHtmlElements
{
	/*****************************************************
	*
	*	Define the "param" from XHTML
	*
	******************************************************/
	class Param : public XHtmlElement
	{
	public:
		Param();
		~Param();
	
	public:
		QString toHtml();
	
	// XHTML attributes valid of this element
	private:
		QString _name;
		QString _mimeType;
		QString _value;
		QString _valueType;
	public:
		QString getName();
		QString getMimeType();
		QString getValue();
		QString getValueType();

		void setName( QString name );
		void setMimeType( QString mimeType );
		void setValue( QString value );
		void setValueType( QString valueType );
	
	};
};

#endif
