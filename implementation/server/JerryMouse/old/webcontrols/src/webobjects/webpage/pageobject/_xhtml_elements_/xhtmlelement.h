#ifndef xhtml_element_h
#define xhtml_element_h

#include <QtCore/QString>

namespace XHtmlElements
{
	/***************************************************************************
	*
	*	Defines the basic XHTML element with all the attributes and events
	*
	****************************************************************************/
	class XHtmlElement
	{
	public:
		XHtmlElement();
		virtual ~XHtmlElement();
	
	public:
		virtual QString toHtml() = 0;

	private: // data to be pushed as element's content. must be XHTML formated already  
		QString _content;
	public:
		QString getContentAsHtml();
		void setContent( QString content );
		
	private: // type definition, used internally by the framework
		QString _type;
	public:
		QString getType();
		void setType( QString type );
		
	protected:
		virtual QString getBasicAttributes();
		
	
 	// attributes related	
	private:
		// XHTML core attributes
		// not valid for: base, head, html, meta, param, script, style, title
		QString _class; // the class of the element
		QString _id;    // a unique id of the element
		QString _style; // a inline style definition
		QString _title  // a text to display as a tooltip
		
		// XHTML language attributes
		// not valid for: base, br, frame, frameset, hr, iframe, param, script
		QString _dir;  // text direction, values: ltr or rtl
		QString _lang; // language code
		
		// XHTML keyboard attributes
		QString _accessKey;
		QString _tabIndex;


	// events related
	private:
		// XHTML mouse events
		// not valid for: base,bdo,br,frame,frameset,head,html,iframe,meta,param,script,style,title
		QString _onClick;
		QString _onDoubleClick;
		QString _onMouseDown;
		QString _onMouseMove;    // all the params mean: script to exec when this happens
		QString _onMouseOver;
		QString _onMouseOut;
		QString _onMouseUp;
		
		// XHTML keyboard events
		// not valid for: base,bdo,br,frame,frameset,head,html,iframe,meta,param,script,style,title
		QString _onKeyDown;
		QString _onKeyPress;    // all the params mean: script to exec when this happens
		QString _onKeyUp;


	// interfaces for basic events/attributes
	public:
		// core attributes
		QString getClass();
		QString getID();
		QString getStyle();
		QString getTitle();
		void setClass( QString cllass );
		void setID( QString id );
		void setStyle( QString syle );
		void setTitle( QString title );

		// language attributes
		QString getDir();
		QString getLang();
		void setDir( QString dir );
		void setLang( QString lang );

		// keyboard attributes
		QString getAccessKey();
		QString getTabIndex();
		void setAccessKey( QString accessKey );
		void setTabIndex( QString tabIndex );

		// mouse events
		QString getOnClick();
		QString getOnDoubleClick();
		QString getOnMouseDown();
		QString getOnMouseMove();
		QString getOnMouseOver();
		QString getOnMouseOut();
		QString getOnMouseUp();
		void setOnClick( QString onClick );
		void setOnDoubleClick( QString onDoubleClick );
		void setOnMouseDown( QString onMouseDown );
		void setOnMouseMove( QString onMouseMove );
		void setOnMouseOver( QString onMouseOver );
		void setOnMouseOut( QString onMouseOut );
		void setOnMouseUp( QString onMouseUp );

		// keyboard events
		QString getOnKeyDown();
		QString getOnKeyPress();
		QString getOnKeyUp();
		void setOnKeyDown( QString onKeyDown );
		void setOnKeyPress( QString onKeyPress );
		void setOnKeyUp( QString onKeyUp );
	
	};
};

#endif
