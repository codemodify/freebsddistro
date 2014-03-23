#ifndef RESIZE_FRAME_H
#define RESIZE_FRAME_H

// Qt headers
#include <QtGui/QLabel>

class Client;

class ResizeFrame : public QLabel
{

public:
	ResizeFrame( Client* parent = 0, const char* name = 0 );


protected:
	void mousePressEvent( QMouseEvent* mouseEvent );
	void mouseReleaseEvent( QMouseEvent* mouseEvent );
	void mouseMoveEvent( QMouseEvent* mouseEvent );


private:
	Client* _client;
};

 #endif
