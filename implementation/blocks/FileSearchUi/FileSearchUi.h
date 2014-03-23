#ifndef __FileSearchUi_H__
#define __FileSearchUi_H__

// Qt headers
#include <QtGui/QListView>
#include <QtGui/QWidget>

// forwards
class FileSearch;
class QLineEdit;
class QPushButton;


class FileSearchUi : public QWidget
{
	Q_OBJECT

	public:
		FileSearchUi( FileSearch* fileSearch );
		~FileSearchUi();

		void setSearchFolder( QString folder );

	signals:
		void fileDoubleClicked( QString file );

	private slots:
		void slot_DoubleClicked( const QModelIndex& modelIndex );
		void slot_Search();

	private:
		QLineEdit*	_where;
		QLineEdit*	_nameFilter;
		QLineEdit*	_contentFilter;
		QPushButton*	_search;

		QListView*	_result;

		FileSearch* 	_fileSearch;
};

#endif
