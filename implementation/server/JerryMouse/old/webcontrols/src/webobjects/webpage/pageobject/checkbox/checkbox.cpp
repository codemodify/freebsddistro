
#include "checkbox.h"

CheckBox::CheckBox() :
	Label(),
	HtmlElements::Check()
{}

CheckBox::~CheckBox()
{}

QString CheckBox::toHtml()
{
	QString asHtml;
	
	asHtml = HtmlElements::Check::toHtml();
	asHtml+= Label::toHtml();
	
	return asHtml;	
}
