
// local headers
#include "ToolWidget.h"
#include "FileEditor.Content.h"

ToolWidget::ToolWidget( FileEditorContent* fileEditorContent ) :
	QWidget()	
{
	connect( this, SIGNAL(ready()), fileEditorContent, SLOT(addToolWidget()) );
}

