#ifndef edit_h
#define edit_h

#include "../input.h"

namespace XHtmlElements
{
	/*****************************************************
	*
	*	Define the "input edit" from XHTML
	*
	******************************************************/
	class Edit : public Input
	{
	public:
		Edit();
		~Edit();
		
		QString toHtml();

	// XHTML attribute valid for this element only		
	private:
		bool _readOnly;
		
	public
		bool getReadOnly();
		void setReadOnly();
	
	};
};

#endif
