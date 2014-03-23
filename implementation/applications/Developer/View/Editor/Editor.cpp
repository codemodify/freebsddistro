
// local headers
#include "Editor.h"

Editor::Editor( QString filePath ):
	FileEditor( filePath )
{
	setFeatures( QDockWidget::DockWidgetClosable );
}
