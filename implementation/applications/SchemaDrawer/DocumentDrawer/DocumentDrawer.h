#ifndef __DocumentDrawer_H__
#define __DocumentDrawer_H__

// Qt headers
#include <QtCore/QList>
#include <QtGui/QGraphicsView>
#include <QtGui/QMenu>

// forwards
class PageDrawer;
class QTabBar;
class QFrame;


class DocumentDrawer : public QGraphicsView
{
	Q_OBJECT

	public:
		DocumentDrawer();
		~DocumentDrawer();

	// for the public
	public:
		void saveDocument();
		void openDocument( QString fileName );

	// inherited stuff
	protected:
		bool eventFilter( QObject* object, QEvent* event );
		void resizeEvent( QResizeEvent* event            );

	// internal stuff
	private slots:
		void pageChanged( int index );
		void tabBarContextMenu( const QPoint& point );

		void addNewPage();
		void removePage();
		void setPageText();
		void removeEmptyPages();
		void removeAllPages();

	private:
		int 					_pageCount;
		QList<PageDrawer*>		_pageDrawerList;
		QString				_fileName;

		QMenu				_menu;
		QTabBar*			 	_tabBar;
		QFrame*				_buttonsFrame;
		bool					_wheelSwitch;
};

#endif
