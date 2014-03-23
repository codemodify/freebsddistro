#ifndef __SearchResult_H__
#define __SearchResult_H__

// local headers
#include "View/View.h"

// forwards
class FileSearch;
class FileSearchUi;

class SearchResult : public View 
{
	Q_OBJECT

	public:
		SearchResult();
		~SearchResult();

	signals:
		void editFile( QString filePath );
	private slots:
		void slot_EditFile( QString filePath );

	public:
		void setSearchFolder( QString folder );

	private:
		FileSearch* 	_fileSearch;
		FileSearchUi*	_fileSearchUi;
};

#endif
