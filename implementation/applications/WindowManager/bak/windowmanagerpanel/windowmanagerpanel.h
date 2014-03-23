
// local headers
#include "panel/panel.h"

class WindowmanagerPanel : public Panel
{

	Q_OBJECT

public:
	WindowmanagerPanel();
	~WindowmanagerPanel();


	void addWindow( QString );
};
