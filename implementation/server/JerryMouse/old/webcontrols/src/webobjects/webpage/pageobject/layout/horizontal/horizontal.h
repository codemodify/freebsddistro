#ifndef HORIZONTAL_LAYOUT_H
#define HORIZONTAL_LAYOUT_H

#include "../layout.h"

class HorizontalLayout : public Layout
{
public:
	HorizontalLayout();
	~HorizontalLayout();
	
public:
	QString toHtml();

};

#endif
