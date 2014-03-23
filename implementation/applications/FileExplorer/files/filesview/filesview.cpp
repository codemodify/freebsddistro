#include "filesview.h"

FilesView::FilesView( QWidget* parent ):
	QListView( parent )
{
	setAcceptDrops( true );
}

FilesView::~FilesView()
{
}

void FilesView::dragEnterEvent( QDragEnterEvent * event)
{
	event->accept();
}

void FilesView::dragMoveEvent(QDragMoveEvent *event)
{
	event->accept();
}

void FilesView::dropEvent( QDropEvent* event )
{
	emit drop( event );
}

void FilesView::keyPressEvent ( QKeyEvent * event )
{
	emit keyPress( event );
}
