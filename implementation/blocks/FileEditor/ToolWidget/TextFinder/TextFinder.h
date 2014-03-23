#ifndef __TextFinder_H__
#define __TextFinder_H__

// Qt headers
#include <QtGui/QLineEdit>
#include <QtGui/QCheckBox>

// local headers
#include "ToolWidget/ToolWidget.h"

// forwards
class FileEditorContent;
class QTextCursor;
class QPlainTextEdit;


class TextFinder : public ToolWidget
{
	Q_OBJECT

	public:
		TextFinder( FileEditorContent* fileEditorContent );
		~TextFinder();

	private slots:
		void slot_PushButtonPressed();

	private:
		QLineEdit* _textToFind;
		QCheckBox* _findBackward;
		QCheckBox* _findCaseSensitive;
		QCheckBox* _findWholeWords;

	private:
		QPlainTextEdit* _textFileEditor;
};

#endif 
