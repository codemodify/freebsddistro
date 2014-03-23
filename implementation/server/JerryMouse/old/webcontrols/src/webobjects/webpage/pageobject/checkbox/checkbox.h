#ifndef CHECKBOX_H
#define CHECKBOX_H

#include "../label/label.h"
#include "../htmlelements/check/check.h"

class CheckBox : public Label, public HtmlElements::Check
{
public:
	CheckBox();
	~CheckBox();
	
	QString toHtml();
	
};

#endif
