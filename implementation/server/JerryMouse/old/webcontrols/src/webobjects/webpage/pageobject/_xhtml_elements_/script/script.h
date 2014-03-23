#ifndef script_h
#define script_h

#include "../xhtmlelement.h"

namespace XHtmlElements
{
	/*****************************************************
	*
	*	Define the "script" from XHTML
	*
	******************************************************/
	class Script : public XHtmlElement
	{
	public:
		Script();
		~Script();
	
	public:
		QString toHtml();
	
	// XHTML attributes valid of this element
	private:
		QString _scriptType;
		QString _src;
	public:
		QString getScriptType();
		QString getSrc();
		void setScriptType( QString sciptType );
		void setSrc( QString src );
	
	};
};

#endif
