#ifndef DESKTOPWINDOW_H_
#define DESKTOPWINDOW_H_

#include <QtCore/QObject>


/*********************************************************************************************
 *
 *	This class implements a X11 window that is visible and accesible to user to work with.
 *
 *	It makes easier the management of X11 window.
 * 
 *********************************************************************************************/
class DesktopWindow : public QObject
{

public:
	DesktopWindow( Window window, Display* );
	~DesktopWindow();

	
	//
	// Interfaces to use to manage a X11 window
	//


public: // x11-window data
	Window getWindowId(){ return _x11WindowId; }
private:
	Window _x11WindowId;
	Display* _x11Display;



public: // title text related
	QString getTitleText(){ return _titleText; }	
	void setTitleText( QString titleText );	
private:
	QStirng _titleText;



public: // icon related
	QPixmap getIcon(){ return _icon; }
private:
	QPixmap _incon;



public: // maximize related	
	bool isMaximized(){ return _isMaximized; }
	void maximize();
	void unmaximize();
private:
	bool _isMaximized;



public: // minimize related
	bool isMinimized(){ return _isMinimized; }
	void minimize()
	void unminimize();
private:
	bool _isMinimized;



public: // shade related
	bool isShaded(){ return _isShaded; }
	void sahde();
	void unshade();
private:
	bool _isShaded;



public: // size related
	QSize getSize(){ return _size; }
	void setSize( QSize size );
private:
	QSize _size;



public: // on-top related
	bool isOntop(){ return _isOntop; }
	void setOntop( bool ontop );
private:
	bool _isOntop;


};

#endif
























