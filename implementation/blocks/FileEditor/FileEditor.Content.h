#ifndef __FileEditor_Content_H__
#define __FileEditor_Content_H__

// Qt headers
#include <QtGui/QWidget>
#include <QtGui/QPlainTextEdit>

// forward declaration
class QAction;
class TextFileEditor;
class QVBoxLayout;
class QToolBar;


class FileEditorContent : public QWidget
{
	Q_OBJECT

	public:
		FileEditorContent( QString filePath );

        QPlainTextEdit* textEditor();

	
	signals: // inform about file modifications
		void fileModified();
		void fileSaved();

	private slots:
        void find();
		void addToolWidget();
		void fileWasChanged( bool modified );

	private:
        TextFileEditor* _textFileEditor;
		QVBoxLayout*    _toolWidgets;

};

#endif
