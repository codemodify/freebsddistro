#ifndef button_h
#define button_h

#include "../pageobject.h"

class Button : public PageObject
{
public:
	Button();
	~Button();

	QString toHtml();
	
	void setText( QString text );
	void setImage( QString imagePath );

private:
	 QString _text;
	 QString _imagePath;
};

#endif
