#ifndef _FILESVIEW_H_
#define _FILESVIEW_H_

#include <QtGui/QWidget>
#include <QtGui/QDropEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QListView>
#include <QtGui/QDragEnterEvent>

class FilesView : public QListView
{
	Q_OBJECT
	
public:
	FilesView( QWidget* parent = 0 );
	~FilesView();

signals:
	void drop( QDropEvent *event );
	void keyPress( QKeyEvent * event );
	
protected:

	void dragEnterEvent( QDragEnterEvent* event );
	void dragMoveEvent(QDragMoveEvent *event);
	void dropEvent( QDropEvent* event );
	void keyPressEvent ( QKeyEvent * event );
};

#endif
