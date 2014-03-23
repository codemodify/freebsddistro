#ifndef LABEL_H
#define LABEL_H

#include "../pageobject.h"

class Label : public PageObject
{
public:
	Label();
	~Label();
	
	void setText( QString text );
	void setBold( bool bold );
	void setItalic( bool italic );
	void setUnderlined( bool underlined );
	void setCrossed( bool crossed );
	void setSize( int size );
	void setFont( QString font );
	void setColor( QString color );
	
	QString toHtml();
	
private:
	QString _text;
	bool _bold;
	bool _italic;
	bool _underlined;
	bool _crossed;
	int _size;
	QString _font;
	QString _color;	
	
};

#endif
