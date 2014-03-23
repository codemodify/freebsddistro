#ifndef CLIENT_H
#define CLIENT_H

// Qt headers
#include <QtGui/QFrame.h>
#include <QtGui/QPushButton.h>
#include <QtCore/QPoint.h>


enum
{
	OP_MAXIMIZE,
	OP_ICONIFY,
	OP_MOVE,
	OP_RESHAPE,
	OP_DESTROY
};




class Client : public QFrame
{

	Q_OBJECT

public:
	Client( QWidget* parent = 0, const char*name = 0 );


public:
	void  craphack( int );  
	Window      window;
	Window      parent_window;
	Bool already_managed;
	Bool already_got_create_event;

	Window      trans;
	Client      *next;
	Client      *revert;

	/* Virtual Desktop key */
	int         vd;
  
	int         x;
	int         y;
	int         dx;
	int         dy;
	int         border;
 
	// for the unmaximize function
	int         x2;
	int         y2;
	int         dx2;
	int         dy2;

  
  
	XSizeHints  size;
	int         min_dx;
	int         min_dy;
 
	int         state;
	int         init;
	int         reparenting;
	int         hold;
	int         proto;
	int         id;
  
	char        *label;
	char        *instance;
	char        *klass;
	char        *name;
	char        *iconname;
  
	Colormap    cmap;
	int         ncmapwins;
	Window      *cmapwins;
	Colormap    *wmcmaps;

	int         sticky;
  
	QPushButton   *buttonA;
	QPushButton   *buttonB;
	QPushButton   *buttonC;
	QPushButton   *buttonD;
	QPushButton   *buttonE;
	QPushButton   *buttonF;
 
	ResizeFrame    *rightResizeFrame;

	void setactive(Bool on);
	void setlabel(char*);

	void handle_operations_popup(int);


 
public slots:
	void minimize_clicked();
	void maximize_toggled(bool);
	void close_clicked();
	void pushpin_toggled(bool);
	void sorry();
	void unMinimize();
	void switchVD();


signals:
	void clientClosed( int );

   
protected:
	void resizeEvent( QResizeEvent * );
	void mousePressEvent( QMouseEvent * );
	void mouseReleaseEvent( QMouseEvent * );
	void mouseMoveEvent( QMouseEvent * );
	void paintEvent( QPaintEvent * );


private:
	QPushButton * getNewButton( int ); 
	QPoint old_cursor_pos;
	Bool is_active;

	// for dragging
	enum DRAGGING
	{
		dragging_enabled,
		dragging_smooth_wait,
		dragging_wait_for_click,
		dragging_nope,
		dragging_resize_wait,
		dragging_resize
	};
	DRAGGING dragging_state;

	// for the titlebar
	QRect title_rect;
	QRect miniicon_rect;
    
};


extern Client       *clients;
extern Client       *current;


//void    setactive(Client*, int);
void    draw_border(Client*, int);
void    active(Client*);
void    nofocus();
Client  *getclient(Window w, int create);
void    rmclient(Client *c);



class OperationsHandler:public QObject
{
	Q_OBJECT

public:
    OperationsHandler( QWidget *parent=0, const char *name=0 ):QObject(parent, name){};

public slots:
	void handle_operations_popup(int i){ current->handle_operations_popup(i); }

};

#endif
