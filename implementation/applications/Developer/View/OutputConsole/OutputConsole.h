#ifndef __OutputConsole_H__
#define __OutputConsole_H__

// Qt's headers
#include <QtGui/QPlainTextEdit>

// local headers
#include "../View.h"

class QTabWidget;


class OutputConsole : public View
{
	Q_OBJECT

	public:
		OutputConsole();
		~OutputConsole();

	public slots:
		void outputConsoleAdd       ( QString output );
         void outputConsoleAddWarning( QString output );
         void outputConsoleAddError  ( QString output );
		void outputConsoleClear     ();

	private:
		QPlainTextEdit	_infoOutput;
		QPlainTextEdit	_warningOutput;
		QPlainTextEdit	_errorOutput;
		QTabWidget*		_tabWidget;

};

#endif
