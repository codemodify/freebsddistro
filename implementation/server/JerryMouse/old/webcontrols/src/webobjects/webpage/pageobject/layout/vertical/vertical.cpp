
#include "vertical.h"

#include "../../htmlelements/table/table.h"
#include "../../htmlelements/tbody/tbody.h"
#include "../../htmlelements/td/td.h"
#include "../../htmlelements/tr/tr.h"

VerticalLayout::VerticalLayout():
	Layout()
{}

VerticalLayout::~VerticalLayout()
{}

QString VerticalLayout::toHtml()
{
	QString asHtml;
	
	QListIterator<PageObject*> listIterator( _containedObjects );
	while( listIterator.hasNext() )
	{	
		PageObject* pageObject = listIterator.next();
		
		HtmlElements::Td td;
		td.setContent( pageObject->toHtml() );
		
		HtmlElements::Tr tr;
		tr.setContent( td.toHtml() );
		
		asHtml += tr.toHtml();
	}
		
	HtmlElements::TBody tbody;
	tbody.setContent( asHtml );
	
	HtmlElements::Table table;
	table.setContent( tbody.toHtml() );
	
	return table.toHtml();
}
