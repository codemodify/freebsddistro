#ifndef __FileEditor_H__
#define __FileEditor_H__

// blocks headers
#include <CustomDockWidget/CustomDockWidget.h>

// forwards
class FileEditorContent;
class QAction;
class QToolBar;


class FileEditor : public CustomDockWidget
{
	Q_OBJECT

	public:
		FileEditor( QString filePath="", QWidget* parent=0 );
        QString getFilePath();

	// inform about file modifications
	signals:
		void fileModified();
		void fileSaved();

    private slots:
        void setDocumentReadOnly();

	protected:
		void closeEvent( QCloseEvent* event );

    private:
        QString             _filePath;

        QToolBar*           _titleBar;
		QAction*            _actionReadonly;
		QAction*            _actionSave;
		QAction*            _actionUndo;
		QAction*            _actionRedo;
		QAction*            _actionSearch;
        FileEditorContent*  _editorWidget;

};

#endif

