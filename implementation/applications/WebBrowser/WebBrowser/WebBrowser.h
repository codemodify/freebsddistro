#ifndef __WebBrowser_H__
#define __WebBrowser_H__

#include <QtGui/QMainWindow>

class QTabWidget;
class QPushButton;
class QLineEdit;
class QUrl;

class WebBrowser : public QMainWindow
{

	Q_OBJECT


	public:
		WebBrowser();
		~WebBrowser();

	private:
		void initGui();

	private slots:
		void newTab();
		void closeTab();
		void tabActivated( int tabIndex );
		void goToAddress();

		void titleChanged( const QString& newTitle );
		void urlChanged( const QUrl& newUrl );


	private:
		QPushButton* 	_back;
		QPushButton* 	_stop;
		QPushButton* 	_reload;
		QPushButton* 	_forward;
		QPushButton* 	_newTab;
		QPushButton* 	_closeTab;
		QLineEdit*	_address;
		QPushButton*	_go;

		QTabWidget* _tabWidget;
};

#endif
