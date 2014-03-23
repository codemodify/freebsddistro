#ifndef GUI_H
#define GUI_H

// Qt headers
#include <QtGui/QWidget>

// local headers
#include "ui_gui.h"

class Gui : public QWidget, public Ui::Form
{

	Q_OBJECT

public:
	Gui();

private slots:
	void setDisplay();
	void createWindow();
	void showWindow();
	void setWindowTitleText();
	void getWindowTitleText();
	void hideWindow();
	void focusWindow();
	void bringWindowOnTop();
	void sendWindowToBack();

};

#endif
