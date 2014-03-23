#ifndef meta_h
#define meta_h

#include "../xhtmlelement.h"

namespace XHtmlElements
{ 
	/*****************************************************
	*
	*	Define the "meta" tag from XHTML
	*
	******************************************************/

	class Meta : public XHtmlElement
	{
		public:
			Meta();
			~Meta();
			
		public:
			QSttring toHtml();
			
		// XHTML attributes valid for this element
		private:
			QString _content;
			QString _httpEquiv;
			QString _name;
			QString _scheme;
		public:
			QString getContent();
			QString getHttpEquiv();
			QString getName();
			QString getScheme();

			bool getContent( QString content );
			bool getHttpEquiv( QString httpEquiv );
			bool getName( QString name );
			bool getScheme( QString scheme );

	};
};

#endif
