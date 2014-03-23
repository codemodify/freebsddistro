#ifndef linenumbers_h
#define linenumbers_h

// Qt hedaers
#include <QtGui/QWidget>

// forward delcarations
class TextFileEditor;


class LineNumbers: public QWidget
{
	Q_OBJECT

	public:
		LineNumbers( TextFileEditor* textFileEditor );

	protected:
		void paintEvent( QPaintEvent* event );
		void resizeEvent( QResizeEvent *e );

	private:
		TextFileEditor* _textFileEditor;
};

#endif
