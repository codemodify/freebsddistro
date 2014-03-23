#ifndef TITLEBAR_H
#define TITLEBAR_H

// Qt's headers
#include <QtGui/QWidget>
#include <QtCore/QPointer>
#include <QtCore/QPoint>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QToolButton>
#include <QtGui/QMouseEvent>

#define TITLE_HEIGHT 20

class TitleBar : public QWidget
{

	Q_OBJECT

public:
	TitleBar();
	~TitleBar();

	void setTitlebarText( QString text );
	QString getTitlebarText();


protected:
	QPointer<QHBoxLayout> _horizontalLayout;
	QPointer<QLabel> _icon;
	QPointer<QLabel> _text;
	QPointer<QToolButton> _minimizeButton;
	QPointer<QToolButton> _maximizeButton;
	QPointer<QToolButton> _closeButton;


protected: // implementing the move-window action
	void mousePressEvent( QMouseEvent* event );
	void mouseMoveEvent( QMouseEvent* event ); 
	QPoint _dragPosition;


signals:
	void minimizeButton();
	void maximizeButton();
	void closeButton();

	void titleDragged( int x, int y );
};

#endif
