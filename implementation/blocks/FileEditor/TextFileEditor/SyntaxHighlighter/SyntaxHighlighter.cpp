
// Qt headers
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomNamedNodeMap>
#include <QtGui/QApplication>

// local headers
#include "SyntaxHighlighter.h"
#include "../TextFileEditor.h"

// the syntax resources
#define SYNTAX_RESOURCES_PATH ( QApplication::applicationDirPath() + "/resources/syntax-highlight/" )

SyntaxHighlighter::HighlightingRules SyntaxHighlighter::_highlightingRules;

SyntaxHighlighter::SyntaxHighlighter( TextFileEditor* parent, QString filePath ):
	QSyntaxHighlighter(parent),
	_textEditor( parent )
{
    setDocument( parent->document() );
	loadSyntaxForFile( filePath );
}

SyntaxHighlighter::~SyntaxHighlighter()
{
/* FIXME: if we add this thing here then at runtime the syntax disappears

	QHashIterator<FileType,HighlightMode*> hashIterator( _highlightingRules );
	while( hashIterator.hasNext() )
	{
		hashIterator.next();

		HighlightMode* highlightMode = hashIterator.value();
		highlightMode->listOfHighlightRules.clear();
		delete highlightMode;
	}

	_highlightingRules.clear();	
*/
}

bool SyntaxHighlighter::isFileTypeLoaded()
{
	return _highlightingRules.contains( _fileType );
}

void SyntaxHighlighter::addRule( HighlightRule& highlightingRule )
{
	if( isFileTypeLoaded() )
	{
		HighlightMode* highlightMode = _highlightingRules[_fileType];
		if( 0 != highlightMode )
			highlightMode->listOfHighlightRules.append( highlightingRule );
	}
}

void SyntaxHighlighter::highlightBlock( const QString& text )
{
	if( false == isFileTypeLoaded() )
		return;

	// getting the highlight scema for out file type
	HighlightMode* highlightMode = _highlightingRules[_fileType];
	if( 0 == highlightMode )
		return;

	foreach( HighlightRule highlightingRule, highlightMode->listOfHighlightRules )
	{
		int index = 0;
		while( index = text.indexOf(highlightingRule.pattern,index), index>=0 )
		{
			int length = highlightingRule.pattern.matchedLength();
			setFormat( index, length, highlightingRule.format );
			index += length;
		}
	}

	// here we apply the rules in case of multi line comments.
	const int noMultiLineComment = 0;
	const int multiLineComment = 1;

	// if no comments till now, look for it in current block
	if( previousBlockState() != multiLineComment )
	{
		int starterIndex = text.indexOf( highlightMode->multilineCommentStart, 0 );
		if( -1 == starterIndex )
		{
			setCurrentBlockState( noMultiLineComment );
		}
		else
		{
			int terminatorIndex = text.indexOf( highlightMode->multilineCommentEnd, starterIndex );
			if( -1 != terminatorIndex )
			{
				setFormat( starterIndex, terminatorIndex-starterIndex, highlightMode->multilineCommentFormat );
				setCurrentBlockState( noMultiLineComment );
			}
			else
			{
				setFormat( starterIndex, text.length()-starterIndex, highlightMode->multilineCommentFormat );
				setCurrentBlockState( multiLineComment );
			}
		}
	}

	// if the previoius block states that it contains the '/*' then in current blok we look for '*/'
	else if( previousBlockState() == multiLineComment )
	{
		int terminatorIndex = text.indexOf( highlightMode->multilineCommentEnd, 0 );

		// if we did not found it then we're still in the middle of the comment
		if( -1 == terminatorIndex )
		{
			setCurrentBlockState( multiLineComment );
			setFormat( 0, text.length(), highlightMode->multilineCommentFormat );
		}
		// we found the comment terminator
		else
		{
			setCurrentBlockState( noMultiLineComment );
			setFormat( 0, terminatorIndex + 2, highlightMode->multilineCommentFormat );
		}
	}
}

void SyntaxHighlighter::highlightBlock2( const QString &text )
{
    QTextCharFormat format;
            		format.setForeground( QBrush(qRgb( 100, 100, 100)) );
            		
	setFormat( 0, text.length(), format );
	
	qDebug( "aaaaaaaaaaaaaaaaaaaaaa" );
	
	return;
	
	

	if( false == isFileTypeLoaded() )
		return;

	// getting the highlight scema for out file type
	HighlightMode* highlightMode = _highlightingRules[_fileType];
	if( 0 == highlightMode )
		return;

	foreach( HighlightRule highlightingRule, highlightMode->listOfHighlightRules )
	{
		int index = 0;
		while( index = text.indexOf(highlightingRule.pattern,index), index>=0 )
		{
			int length = highlightingRule.pattern.matchedLength();
			
			
			QTextCharFormat format;
			format.setForeground( QBrush(qRgb( 100, 100, 100)) );
			
			setFormat( index, length, format );
//			setFormat( index, length, highlightingRule.format );
						
			index += length;
		}
	}

/*
    QTextCharFormat emptyFormat;

    const int previousState = previousBlockState();
    int state = 0;
    int braceDepth = 0;
    
    if( previousState != -1 )
    {
        state = previousState & 0xff;
        braceDepth = previousState >> 8;
    }




    int initialState = state;
    const QList<SimpleToken> tokens = tokenize(text, initialState);
    state = tokenize.state(); // refresh the state

    if( tokens.isEmpty() )
    {
        setCurrentBlockState(previousState);
        if (TextBlockUserData *userData = TextEditDocumentLayout::testUserData(currentBlock())) {
            userData->setClosingCollapseMode(TextBlockUserData::NoClosingCollapse);
            userData->setCollapseMode(TextBlockUserData::NoCollapse);
        }
        TextEditDocumentLayout::clearParentheses(currentBlock());
        return;
    }

    TextEditDocumentLayout::setParentheses(currentBlock(), parentheses);

    setCurrentBlockState((braceDepth << 8) | tokenize.state());
*/
}


QDomNode SyntaxHighlighter::getChildByName( QDomNode parent, QString nodeName )
{	
	// getting the first child of the parent
	QDomNode node = parent.firstChild();
	
	// searching the requested node, by his name
	while( (false == node.isNull()) && (node.toElement().tagName() != nodeName) )
		node = node.nextSibling();
	
	// returning the found result
	return node;
}

void SyntaxHighlighter::addHighlightRuleForPatternsFromNode( QDomNode& node )
{
	if( node.isNull() )
		return;

	// getting the color to use
	QString colorAsString = node.attributes().namedItem("color").nodeValue();
	if( colorAsString.isEmpty() )
		return;

	QColor color;
	color.setNamedColor( colorAsString );

	// checking what kind of rule we must use
	HighlightMode* highlightMode = _highlightingRules[_fileType];
	if( 0 == highlightMode )
		return;

	HighlightRule highlightingRule;

	if( node.toElement().tagName() == "identifiers" )
	{
		highlightingRule.pattern = QRegExp( "\\w", Qt::CaseSensitive, QRegExp::RegExp );
		highlightingRule.format.setForeground( QBrush(color) );
		highlightMode->listOfHighlightRules.append( highlightingRule );
	}
	else if( node.toElement().tagName() == "numbers" )
	{
		highlightingRule.pattern = QRegExp( "\\b\\d{1,1000}\\b", Qt::CaseSensitive, QRegExp::RegExp );
		highlightingRule.format.setForeground( QBrush(color) );
		highlightMode->listOfHighlightRules.append( highlightingRule );
	}
	else if( node.toElement().tagName() == "strings" )
	{
		highlightingRule.pattern = QRegExp( "\".*\"", Qt::CaseSensitive, QRegExp::RegExp );
		highlightingRule.format.setForeground( QBrush(color) );
		highlightMode->listOfHighlightRules.append( highlightingRule );
	}
	else if( node.toElement().tagName() == "macros" )
	{
		highlightingRule.pattern = QRegExp( node.toElement().text() + "[^\n]*", Qt::CaseSensitive, QRegExp::RegExp );
		highlightingRule.format.setForeground( QBrush(color) );
		highlightMode->listOfHighlightRules.append( highlightingRule );
	}
	else if( node.toElement().tagName() == "singleLine" )
	{
		if( node.toElement().text().isEmpty() )
			return;

		highlightingRule.pattern = QRegExp( node.toElement().text() + "[^\n]*", Qt::CaseSensitive, QRegExp::RegExp );
		highlightingRule.format.setForeground( QBrush(color) );
		highlightingRule.format.setFontItalic( true );
		highlightMode->listOfHighlightRules.append( highlightingRule );
	}
	else if( node.toElement().tagName() == "multiLine" )
	{
		QStringList multiLineComment = node.toElement().text().split(" ");
		if( multiLineComment.size() != 2 )
			return;

		highlightMode->multilineCommentStart = multiLineComment.at( 0 );
		highlightMode->multilineCommentEnd = multiLineComment.at( 1 );
		highlightMode->multilineCommentFormat.setForeground( QBrush(color) );
		highlightMode->multilineCommentFormat.setFontItalic( true );
	}
	else
	{
		QStringList keywords = node.toElement().text().split(" ");
		foreach( QString keyword, keywords )
		{
			if( true == keyword.isEmpty() )
				continue;

			highlightingRule.format.setForeground( QBrush(color) );
			highlightingRule.pattern = QRegExp( "\\b" + keyword + "\\b", Qt::CaseSensitive, QRegExp::RegExp );
			highlightMode->listOfHighlightRules.append( highlightingRule );
		}
	}
}

void SyntaxHighlighter::loadSyntaxForFile( QString filePathToHighLight )
{
	// check if we have any suffix at the end of the file
	_fileType = QFileInfo(filePathToHighLight).suffix();
	if( true == _fileType.isEmpty() )
		return;

	// getting here meas we have a file suffix, now we try to load its corresoponding syntax color
	if( true == isFileTypeLoaded() )
		return;

	_highlightingRules.insert( _fileType, new HighlightMode() );
	
	// loading the syntax file for our brand new filetype	
	QFile syntaxFile( SYNTAX_RESOURCES_PATH + _fileType );
	if( false == syntaxFile.open(QFile::ReadOnly) )
		return;

	QDomDocument domDocument;
	if( false == domDocument.setContent(&syntaxFile) )
	{
		syntaxFile.close();
		return;
	}
	syntaxFile.close();


	// now it's time to add syntax-highlighting rules for our file


	// getting the root node and default values for formatting
	QDomNode synatxHighlight = domDocument.documentElement();
	if( true == synatxHighlight.isNull() )
		return;

	// setting the highlight rule for 'identifiers'
	QDomNode identifiers = getChildByName( synatxHighlight, "identifiers" );
	addHighlightRuleForPatternsFromNode( identifiers );
	
	// setting the highlight rule for 'keywords'
	QDomNode keywords = getChildByName( synatxHighlight, "keywords" );
	addHighlightRuleForPatternsFromNode( keywords );

	// setting the highlight rule for 'aditionalKeywords'
	QDomNode aditionalKeywords = getChildByName( synatxHighlight, "aditionalKeywords" );
	QDomNode node = aditionalKeywords.firstChild();
	while( false == node.isNull() )
	{
		addHighlightRuleForPatternsFromNode( node );
		node = node.nextSibling();
	}

	// setting the highlight rule for 'numbers'
	QDomNode numbers = getChildByName( synatxHighlight, "numbers" );
	addHighlightRuleForPatternsFromNode( numbers );
		
	// setting the highlight rule for 'strings'
	QDomNode strings = getChildByName( synatxHighlight, "strings" );
	addHighlightRuleForPatternsFromNode( strings );

	// setting the highlight rule for 'macros'
	QDomNode macros = getChildByName( synatxHighlight, "macros" );
	addHighlightRuleForPatternsFromNode( macros );

	// setting the highlight rule for 'comments'
	QDomNode comments = getChildByName( synatxHighlight, "comments" );

	QDomNode commentsSingleLine = getChildByName( comments, "singleLine" );
	addHighlightRuleForPatternsFromNode( commentsSingleLine );

	QDomNode commentsMultiLine = getChildByName( comments, "multiLine" );
	addHighlightRuleForPatternsFromNode( commentsMultiLine );
}
