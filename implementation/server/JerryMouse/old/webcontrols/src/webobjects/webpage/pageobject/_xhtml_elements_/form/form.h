#ifndef form_h
#define form_h

#include "../xhtmlelement.h"

namespace XHtmlElements
{
	/*****************************************************
	*
	*	Define the "form" element
	*
	******************************************************/
	class Form : public XHtmlElement
	{
	public:
		Form();
		~Form();
		
	public:
		QString toHtml();
	
	// XHTML events that are valid only for this element
	private:
		QString _onChange;
		QString _onSubmit;
		QString _onReset;
		QString _onSelect;
		QString _onBlur;
		QString _onFocus;
		
	public:
		QString getOnChange();
		QString getOnSubmit();
		QString getOnReset();
		QString getOnSelect();
		QString getOnBlur();
		QString getOnFocus();
		void setOnChange( QString onChange );
		void setOnSubmit( QString onSubmit );
		void setOnReset( QString onReset );
		void setOnSelect( QString onSelect );
		void setOnBlur( QString onBlur );
		void setOnFocus( QString onFocus );

	
	// XHTML attributes valid for this element only
	private:
		QString _action;
		QString _accept;
		QString _acceptCharset;
		QString _enctype
		QString _method;
		QString _name;
		QString _target;

	public:
		QString getAction();
		QString getAccept();
		QString getAcceptCharset();
		QString getEnctype();
		QString getMethod();
		QString getName();
		QString getTarget();
		void setAction( QString action );
		void setAccept( QString accept );
		void setAcceptCharset( QString acceptCharset );
		void setEnctype( QString enctype );
		void setMethod( QString method );
		void setName( QString name );
		void setTarget( QString type );

	};
};

#endif
