#ifndef __TextFileEditor_H__
#define __TextFileEditor_H__

// Qt headers
#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtGui/QPlainTextEdit>

// forward declaration
class FileEditorContent;
class QMimeData;
class SyntaxHighlighter;


class TextFileEditor : public QPlainTextEdit
{
    Q_OBJECT

    public:
        TextFileEditor( QString filePath );
        ~TextFileEditor();

    public slots:
        void save	();
        void saveAs ();

    // keyboard handlers
    private:
        void handleEscapeKey();
        void handleTabKey();
        void handleShiftTabKey();
        void handleBackspaceKey();
        void handleControlSlash();
        void handleControlBackSlash();

        void handleControlSKey();
        void handleControlXKey();
        void handleControlCKey();
        void handleControlVKey();

        void doIndent( bool doIndent );

    private:
        typedef void (TextFileEditor::*KeyHandler)();
        typedef QMap< int, KeyHandler > KeyHandlers;
        KeyHandlers _keyHandlers;

	// override the inherited stuff
    protected:
        void paintEvent		( 		QPaintEvent*    paintEvent	);
        void insertFromMimeData ( const	QMimeData*      source		);
        void keyPressEvent		( 		QKeyEvent*      event		);

    // helpers
    private:
        void loadFile();

    private:
        QString             _filePath;
        FileEditorContent*  _fileEditorContent;
        SyntaxHighlighter*  _syntaxHighlighter;
};

#endif 
