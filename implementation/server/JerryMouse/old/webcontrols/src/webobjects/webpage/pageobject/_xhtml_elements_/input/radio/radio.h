#ifndef radio_h
#define radio_h

#include "../input.h"

namespace XHtmlElements
{
	/*****************************************************
	*
	*	Define the "input radio" from XHTML
	*
	******************************************************/
	class Radio : public Input
	{
	public:
		Radio();
		~Radio();
		
		QString toHtml();
	
	// XHTML attributes valid for this element
	private:
		bool _checked;
			
	public:
		bool getChecked();
		void setChecked( bool checked );
	
	};
};

#endif
