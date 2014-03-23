#ifndef table_h
#define table_h

#include "../xhtmlelement.h"

namespace XHtmlElements
{
	/*****************************************************
	*
	*	Define the "table" from XHTML
	*
	******************************************************/
	class Table : public XHtmlElement
	{
	public:
		Table();
		~Table();
	
	public:
		QString toHtml();
	
	// XHTML attributes valid of this element
	private:
		QString _border;
		QString _cellPadding;
		QString _cellSpacing;
		QString _frame;
		QString _rules;
		QString _width;
	public:
		QString getBorder();
		QString getCellPadding();
		QString getCellSpacing();
		QString getFrame();
		QString getRules();
		QString getWidth();
		void setBorder( QString border );
		void setCellPadding( QString cellPadding );
		void setCellSpacing( QString cellSpacing );
		void setFrame( QString frame );
		void setRules( QString rules );
		void setWidth( QString width );
	
	};
};

#endif
