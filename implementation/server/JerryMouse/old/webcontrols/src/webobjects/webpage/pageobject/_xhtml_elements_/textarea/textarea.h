#ifndef textarea_h
#define textarea_h

#include "../xhtmlelement.h"

namespace XHtmlElements
{
	/*****************************************************
	*
	*	Define the "iframe" tag from XHTML
	*
	******************************************************/
	class TextArea : public XHtmlElement 
	{
	public:
		TextArea();
		~TextArea();
	
	public:
		QString toHtml();

	// XHTML attributes valid for this element
	private:
		QString _cols;
		QString _rows;
		bool _disabled;
		QString _name;
		bool _readonly;
	public:
		QString getCols();
		QString getRows();
		bool getDisabled();
		QString getName();
		bool getReadonly();
		void setCols( QString cols );
		void setRows( QString rows );
		void setDisabled( bool disabled );
		void setName( QString name );
		void setReadonly( bool readonly );

	};
};

#endif
