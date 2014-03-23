
#include "groupbox.h"

#include "../htmlelements/fieldset/fieldset.h"


GroupBox::GroupBox() :
	PageObject()
{}

GroupBox::~GroupBox()
{}

QString GroupBox::toHtml()
{
	HtmlElements::Fieldset fieldset;
	fieldset.setContent( getContainedObjectsAsHTML() );

	QString asHtml = fieldset.toHtml();

	return asHtml;
}
