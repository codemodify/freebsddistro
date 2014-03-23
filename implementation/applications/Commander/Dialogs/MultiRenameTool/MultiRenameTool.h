#ifndef __MultiRenameTool_H__
#define __MultiRenameTool_H__

// Qt headers
#include <QtGui/QWidget>

// forwards
class QPushButton;
class QGroupBox;
class QLineEdit;
class QSpinBox;
class QListWidget;
class QRadioButton;


class MultiRenameTool : public QWidget
{
    Q_OBJECT

    public:
        MultiRenameTool( QStringList& itemsToRename );
        ~MultiRenameTool();

    protected:
        void closeEvent( QCloseEvent* event );

    private slots:
        void preview();
        void apply();

    private:
        QStringList     _itemsToRename;

        // name
        QGroupBox*      _nameGroup;

        QLineEdit*      _namePre;

        QGroupBox*      _nameRemoveGroup;
        QSpinBox*       _nameRemoveStartIndex;
        QSpinBox*       _nameRemoveEndIndex;

        QGroupBox*      _nameIndexGroup;
        QSpinBox*       _nameIndexStartIndex;

        QGroupBox*      _nameCaseGroup;
        QRadioButton*   _nameLowerCase;
        QRadioButton*   _nameUpperCase;

        QLineEdit*      _namePost;

        // suffix
        QGroupBox*      _suffixGroup;

        QLineEdit*      _suffixPre;

        QGroupBox*      _suffixRemoveGroup;
        QSpinBox*       _suffixRemoveStartIndex;
        QSpinBox*       _suffixRemoveEndIndex;

        QGroupBox*      _suffixIndexGroup;
        QSpinBox*       _suffixIndexStartIndex;

        QGroupBox*      _suffixCaseGroup;
        QRadioButton*    _suffixLowerCase;
        QRadioButton*   _suffixUpperCase;

        QLineEdit*      _suffixPost;

        // items
        QListWidget*    _sourceItems;
        QListWidget*    _previewItems;

        // buttons
        QPushButton*    _apply;
        QPushButton*    _cancel;
        QPushButton*    _preview;

};

#endif
