#ifndef VERTICAL_LAYOUT_H
#define VERTICAL_LAYOUT_H

#include "../layout.h"

class VerticalLayout : public Layout
{
public:
	VerticalLayout();
	~VerticalLayout();
	
public:
	QString toHtml();

};

#endif
