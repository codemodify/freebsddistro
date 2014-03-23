#ifndef XWINDOW_H
#define XWINDOW_H

// local headers
#include "helpers/enhancedqwidget/enhancedqwidget.h"
#include "helpers/xlibwrapper/xlibwrapper.h"

class QLabel;
class QToolButton;

class XWindow : public EnhancedQWidget
{

	Q_OBJECT

public:
	XWindow( Window window );
	~XWindow();



public: // interfaces available
	void configureWindow( XConfigureRequestEvent* event );
	void propertyChanged( XPropertyEvent* event );

	// used by the aditional components like TaskBar to display some useful information
	QString getText();

	bool isMinimized();
	bool isOnTop();



public slots: // interfaces available also as slots
	void focus();
	void show();
	void raise();
	void shadeWindow();
	void minimizeWindow();
	void unMinimizeWindow();
	void maximizeWindow();
	void unMaximizeWindow();
	void closeWindow();

	void draggedToNewPosition( QPoint newPosition );



private: // internal data used to manage a wdindow effectively
	Window _window;

	// top part of the GUI elements that are visible
	QToolButton* _shade;
	QLabel* _icon;
	QLabel* _text;
	QToolButton* _minimize;
	QToolButton* _maximize;
	QToolButton* _close;

	// bottom part of the GUI elements that re visible
	EnhancedQWidget* _leftResizeFrame;
	EnhancedQWidget* _rightResizeFrame;
	
	private slots:
	void leftResize( QPoint newPosition );
	void rightResize( QPoint newPosition );

private: // data that indicate some properties for a window
	int _x, _y, _width, _height;

	bool _isShaded;
	bool _isMinimized;
	bool _isMaximized;

	void applyProperties();




protected: // implementing the input focus feature
	void enterEvent( QEvent* event );


};

#endif

