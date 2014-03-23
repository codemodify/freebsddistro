#ifndef __SkinWidget_H__
#define __SkinWidget_H__

// Qt headers
#include <QtCore/QObject>
#include <QtGui/QPixmap>

// forwards
class QWidget;
class QPixmap;

class SkinWidget : public QObject
{
	Q_OBJECT

	public:
		SkinWidget( QWidget* widget );

		virtual void setPixmaps         ( QPixmap& regular, QPixmap& active );
		virtual void setBackgroundPixmap( QPixmap& pixmap );

	protected:
		virtual void setWidgetShape     ( QPixmap& pixmap );

	protected slots:
		void mousePressEvent();
		void mouseReleaseEvent();
		void mouseHoverEnterEvent();
		void mouseHoverLeaveEvent();

		virtual void paintEvent();

	protected:
		QWidget* _widget;

		QPixmap _regular;
		QPixmap _active;

		QPixmap& _current;
};

#endif
