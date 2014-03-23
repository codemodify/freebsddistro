#ifndef tool_widget_h
#define tool_widget_h

// Qt headers
#include <QtGui/QWidget>

class FileEditorContent;


class ToolWidget : public QWidget
{
	Q_OBJECT
	
	public:
		ToolWidget( FileEditorContent* fileEditorContent );

	signals:
		void ready();
};

#endif 
