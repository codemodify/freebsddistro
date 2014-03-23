#ifndef add_remove_selection_h
#define add_remove_selection_h

// Qt headers
#include <QtCore/QPointer>
#include <QtGui/QDialog>

// forward declarations
class QLineEdit;
class QGroupBox;
class QRadioButton;

class AddRemoveSelection : public QDialog
{
    Q_OBJECT

    public:
        AddRemoveSelection( bool addToSelection = true );
        ~AddRemoveSelection();

    public:
        bool haveToDoIt();

        bool extendedOtionsAvailable();
        bool haveToApplyOnlyToFiles();

        QString getCriteria();

    private slots:
        void okClicked();

    private:
        bool _haveToDoIt;

        bool _addToSelection;

        QPointer<QLineEdit> _criteria; 

        QPointer<QGroupBox> _extendedOptions;
        QPointer<QRadioButton> _applyOnlyToFiles;
        QPointer<QRadioButton> _applyOnlyToFolders;
};

#endif
