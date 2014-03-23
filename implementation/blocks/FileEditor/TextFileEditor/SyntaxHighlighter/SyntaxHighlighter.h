#ifndef __SyntaxHighlighter_H__
#define __SyntaxHighlighter_H__

// Qt headers
#include <QtGui/QSyntaxHighlighter>
#include <QtGui/QTextCharFormat>
#include <QtCore/QList>
#include <QtCore/QHash>

// forward declarations
class QDomNode;
class TextFileEditor;

class SyntaxHighlighter : public QSyntaxHighlighter
{

	public:
		SyntaxHighlighter( TextFileEditor* parent, QString filePath );
		~SyntaxHighlighter();

	// from Qt docs we have to override this to be able to do our job
	protected:
		void highlightBlock( const QString& text );
		void highlightBlock2( const QString& text );

	// defines and stores a file type such as: cpp, c, h, python, perl, rubby etc... 
	private:
		typedef QString FileType;
		FileType _fileType;

	// defines the highlighting rules for a file
	private: 
		typedef struct {
			QRegExp pattern;
			QTextCharFormat format;
		} HighlightRule;
		typedef QList<HighlightRule> ListOfHighlightRules;

	// defines a file in highlighed state
	private:
		struct HighlightMode {
			//HighlightRule multilineCommentStart;
			//HighlightRule multilineCommentEnd;
			QString multilineCommentStart;
			QString multilineCommentEnd;
			QTextCharFormat multilineCommentFormat;
			ListOfHighlightRules listOfHighlightRules;
		};

	// defines a list of filetypes, to each filetype corresponds a speciffic way to make highlighting.
	// having this as static object implies that all instances will use the same loaded data when trying to highlight something
	private:
		typedef QHash<FileType,HighlightMode*> HighlightingRules;
		static HighlightingRules _highlightingRules;

	// stores the editor that we work with
	private:
		TextFileEditor* _textEditor;

	// some methods that will facilitate the work with syntax files
	private:
		void addRule( HighlightRule& highlightRule );
		void addHighlightRuleForPatternsFromNode( QDomNode& node );
		void loadSyntaxForFile( QString filePath );
		QDomNode getChildByName( QDomNode parent, QString nodeName );
		bool isFileTypeLoaded();
};

#endif
