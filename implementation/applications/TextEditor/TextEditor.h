#ifndef __TextEditor_H__
#define __TextEditor_H__

// Qt headers
#include <QtGui/QApplication>
#include <QtGui/QTabWidget>
#include <QtGui/QMenu>

class FileTreeView;

class TextEditor : public QTabWidget
{
    Q_OBJECT

    public:
        TextEditor();
        ~TextEditor();


    public slots:
        void newEmptyTab();
        void newTab( QString filePath="" );
        void closeTab();
        void fullScreen();
        void nextTab();
        void previousTab();
        void rightClick( const QPoint& point );
        void explorer();
        void explorerMouseDoubleClick();
        void onTop();


    protected:
        void closeEvent     ( QCloseEvent*      event );

        void dragEnterEvent ( QDragEnterEvent*  event );
        void dropEvent      ( QDropEvent*       event );

        void resizeEvent    ( QResizeEvent*     event );
        void moveEvent      ( QMoveEvent*       event );


    private:
        QMenu            _contextMenu;
        FileTreeView*    _fileTreeView;

};

#endif
