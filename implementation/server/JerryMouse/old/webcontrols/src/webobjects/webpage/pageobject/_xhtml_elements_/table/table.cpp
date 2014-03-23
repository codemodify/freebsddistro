
#include "table.h"

XHtmlElements::Table::Table() :
	XHtmlElement()
{
	setType( "TABLE" );
}

XHtmlElements::Table::~Table()
{}

QString XHtmlElements::Table::toHtml()
{
	QString asHtml = "<table ";
	
	if( false == getBasicAttributes().isEmpty() )
		asHtml+= getBasicAttributes();
		
	if( false == _border.isEmpty() )
		asHtml+= QString(" border=\"%1\" ").arg(_border);

	if( false == _cellPadding.isEmpty() )
		asHtml+= QString(" cellPadding=\"%1\" ").arg(_cellPadding);

	if( false == _cellSpacing.isEmpty() )
		asHtml+= QString(" cellSpacing=\"%1\" ").arg(_cellSpacing);

	if( false == _frame.isEmpty() )
		asHtml+= QString(" frame=\"%1\" ").arg(_frame);

	if( false == _rules.isEmpty() )
		asHtml+= QString(" rules=\"%1\" ").arg(_rules);

	if( false == _width.isEmpty() )
		asHtml+= QString(" width=\"%1\" ").arg(_width);
	
	asHtml+=" >"
	asHtml+= getContentAsHtml();
	
	asHtml+="</table>"; 

	return asHtml;
}

QString XHtmlElements::Table::getBorder(){ return _border; }
QString XHtmlElements::Table::getCellPadding(){ return _cellPadding; }
QString XHtmlElements::Table::getCellSpacing(){ return _cellSpacing; }
QString XHtmlElements::Table::getFrame(){ return _frame; }
QString XHtmlElements::Table::getRules(){ return _rules; }
QString XHtmlElements::Table::getWidth(){ return _width; }

void XHtmlElements::Table::setBorder( QString border ){ _border = border; }
void XHtmlElements::Table::setCellPadding( QString cellPadding ){ _cellPadding = cellPadding; }
void XHtmlElements::Table::setCellSpacing( QString cellSpacing ){ _cellSpacing = cellSpacing; }
void XHtmlElements::Table::setFrame( QString frame ){ _frame = frame; }
void XHtmlElements::Table::setRules( QString rules ){ _rules = rules; }
void XHtmlElements::Table::setWidth( QString width ){ _width = width; }

