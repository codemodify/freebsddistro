
// Qt headers
#include <QtGui/QTabWidget>
#include <QtGui/QAction>

// blocks
#include <IconProvider/ActionIconProvider/ActionIconProvider.h>

// local headers
#include "OutputConsole.h"

static const QString c_outputTabText		= "Output";
static const QString c_warningsTabText	= "Warnings";
static const QString c_errorsTabText		= "Errors";

static const QString c_warningsModifiedTabText	= "* Warnings";
static const QString c_errorsModifiedTabText		= "* Errors";


OutputConsole::OutputConsole():
	View()
{
    setTitleText( "Console" );

    QAction* close = new QAction( 0 );
             close->setIcon( ActionIconProvider().getIconForAction("close") );

    setTitleCloseButton( close, this, SLOT(close()) );

	QFont	font;
			font.setFamily	( "Monospace" );
			font.setPointSize	( 8 );

    QTextCharFormat textCharFormat;
                    textCharFormat.setFont( font );

    textCharFormat.setForeground        ( QBrush(Qt::gray)      );
    _infoOutput.setReadOnly             ( true                  );
    _infoOutput.setCurrentCharFormat    ( textCharFormat        );
    _infoOutput.setPalette              ( QPalette(Qt::darkGray));

    textCharFormat.setForeground        ( QBrush(Qt::darkBlue)  );
    _warningOutput.setReadOnly          ( true                  );
    _warningOutput.setCurrentCharFormat ( textCharFormat        );

    textCharFormat.setForeground        ( QBrush(Qt::darkRed)   );
    _errorOutput.setReadOnly            ( true                  );
    _errorOutput.setCurrentCharFormat   ( textCharFormat        );

    _tabWidget = new QTabWidget();
    _tabWidget->setTabPosition( QTabWidget::West       );
    _tabWidget->setTabShape   ( QTabWidget::Triangular );
    _tabWidget->addTab( &_infoOutput,    c_outputTabText    );
    _tabWidget->addTab( &_warningOutput, c_warningsTabText  );
    _tabWidget->addTab( &_errorOutput,   c_errorsTabText    );
                
	setContentWidget( _tabWidget );
}

OutputConsole::~OutputConsole()
{}

void OutputConsole::outputConsoleAdd( QString output )
{
	_infoOutput.appendPlainText( output.trimmed() );
}

void OutputConsole::outputConsoleAddWarning( QString output )
{
	_warningOutput.appendPlainText( output.trimmed() );

	_tabWidget->setTabText( 1, c_warningsModifiedTabText );
}

void OutputConsole::outputConsoleAddError( QString output )
{
	if
    (
        !output.contains( "warning", Qt::CaseInsensitive ) 
        ||
        output.contains( "error", Qt::CaseInsensitive )
    )
    {
        _errorOutput.appendPlainText( output.trimmed() );
        _tabWidget->setTabText( 2, c_errorsModifiedTabText );
    }

	else
        outputConsoleAddWarning( output );
}

void OutputConsole::outputConsoleClear()
{
	_infoOutput.clear();
	_warningOutput.clear();
	_errorOutput.clear();

	_tabWidget->setTabText( 1, c_warningsTabText );
	_tabWidget->setTabText( 2, c_errorsTabText );
}
