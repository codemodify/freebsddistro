#ifndef doctype_h
#define doctype_h

namespace XHtmlElements
{
	/*****************************************************
	*
	*	Define the "!doctype" element from XHTML
	*
	******************************************************/
	class DocType
	{
	public:
		DocType();
		~DocType();
	
	public:
		QString toHtml();
		
	// XHTML attributes valid for this element only
	typedef enum DocumentType
	{
		Unset, // no DTD types will be stored in the page
		
		Strict, // XHTML Strict DTD
				// clean markup, free of presentational clutter
				// used together with Style Sheets
				
		Transitional, // XHTML Transitional DTD
					  // markup + presentational features
					  // used if the browser do not support Style Sheets
		  
		Frameset // HTML Frameset DTD
				 // used for documents with frames
	};
	private:
		DocumentType _docType;
	public:
		DocumentType getDocType();
		void getDocType( DocumentType documentType );

	};
};

#endif
