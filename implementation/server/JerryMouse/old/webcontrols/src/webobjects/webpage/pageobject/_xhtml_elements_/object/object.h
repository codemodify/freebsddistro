#ifndef object_h
#define object_h

#include "../xhtmlelement.h"

namespace XHtmlElements
{
	/*****************************************************
	*
	*	Define the "object" from XHTML
	*
	******************************************************/
	class Object : public XHtmlElement
	{
	public:
		Object();
		~Object();
	
	public:
		QString toHtml();
	
	// XHTML attributes valid of this element
	private:
		QString _archive;
		QString _classid;
		QStirng _codebase;
		QString _codetype;
		QString _data;
		bool _declare;
		QString _height;
		QString _name;
		QString _standby;
		QString _objectType;
		QString _usemap;
		QString _width;
	public:
		QString getArchive();
		QString getClassid();
		QStirng getCodebase();
		QString getCodetype();
		QString getData();
		bool    getDeclare();
		QString getHeight();
		QString getName();
		QString getStandby();
		QString getObjectType();
		QString getUsemap();
		QString getWidth();

		void setArchive( QString archive );
		void setClassid( QString classid );
		void setCodebase( QString codebase );
		void setCodetype( QString codetype );
		void setData( QString data );
		void setDeclare( bool declare );
		void setHeight( QString height );
		void setName( QString name );
		void setStandby( QString standby );
		void setObjectType( QString type );
		void setUsemap( QString usemap );
		void setWidth( QString width );
	
	};
};

#endif
