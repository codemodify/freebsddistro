#ifndef check_h
#define check_h

#include "../input.h"

namespace XHtmlElements
{
	/*****************************************************
	*
	*	Define the "input check" from XHTML
	*
	******************************************************/
	class Check : public Input
	{
	public:
		Check();
		~Check();
	
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
