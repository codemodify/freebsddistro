#ifndef __Editor_H__
#define __Editor_H__

// blocks
#include <FileEditor/FileEditor.h>

class Editor : public FileEditor
{
	Q_OBJECT

	public:
		Editor( QString filePath );

};

#endif
