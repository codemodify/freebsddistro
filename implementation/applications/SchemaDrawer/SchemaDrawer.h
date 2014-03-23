#ifndef __SchemaDrawer_H__
#define __SchemaDrawer_H__

// Qt headers
#include <QtGui/QMainWindow>
#include <QtGui/QWorkspace>


class SchemaDrawer : public QMainWindow
{
	Q_OBJECT

	public:
		SchemaDrawer();
		~SchemaDrawer();

		void openSchemaByFilePath( QString filePath );

	// inherited stuff
	protected:
		void closeEvent( QCloseEvent* event );

	// menu handlers
	private slots:
		void helpMe();
		
		void newSchema();
		void saveSchema();
		void openSchema();

	private:
		QWorkspace*	_workspace;
};

#endif
