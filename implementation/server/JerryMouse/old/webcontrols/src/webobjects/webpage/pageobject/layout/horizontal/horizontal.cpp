
#include "horizontal.h"

#include "../../htmlelements/table/table.h"
#include "../../htmlelements/tbody/tbody.h"
#include "../../htmlelements/td/td.h"
#include "../../htmlelements/tr/tr.h"

HorizontalLayout::HorizontalLayout():
	Layout()
{}

HorizontalLayout::~HorizontalLayout()
{}

QString HorizontalLayout::toHtml()
{
	QString asHtml;
	
	QListIterator<PageObject*> listIterator( _containedObjects );
	while( listIterator.hasNext() )
	{	
		PageObject* pageObject = listIterator.next();
		
		HtmlElements::Td td;
		td.setContent( pageObject->toHtml() );
		
		asHtml += td.toHtml();
	}
	
	HtmlElements::Tr tr;
	tr.setContent( asHtml );
	
	HtmlElements::TBody tbody;
	tbody.setContent( tr.toHtml() );
	
	HtmlElements::Table table;
	table.setContent( tbody.toHtml() );
	
	return table.toHtml();
}
