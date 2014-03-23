
// Qt headers
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QGroupBox>
#include <QtGui/QRadioButton>
#include <QtGui/QPushButton>


// local headers
#include "AddRemoveSelection.h"

// some local defines
#define TITLE_FOR_ADD_TO_SELECTION "add to selection"
#define TITLE_FOR_REMOVE_FROM_SELECTION "remove from selection"

AddRemoveSelection::AddRemoveSelection( bool addToSelection ) :
    QDialog()
{
    _addToSelection = addToSelection;
    _haveToDoIt = false;

    // set the correct title and modality
    if( _addToSelection )
        setWindowTitle( TITLE_FOR_ADD_TO_SELECTION );
    else
        setWindowTitle( TITLE_FOR_REMOVE_FROM_SELECTION );

    // build the UI controls
    _criteria = new QLineEdit();

    _extendedOptions = new QGroupBox( "extended options" );
    _extendedOptions->setCheckable( true );
    _extendedOptions->setChecked( false );

    _applyOnlyToFiles = new QRadioButton( "apply only to files" );
    _applyOnlyToFiles->setChecked( true );

    _applyOnlyToFolders = new QRadioButton( "apply only to folders" );

    QPointer<QPushButton> okButton = new QPushButton( "ok" );
    connect( okButton, SIGNAL(pressed()), this, SLOT(okClicked()) );

    QPointer<QPushButton> cancelButton = new QPushButton( "cancel" );
    connect( cancelButton, SIGNAL(pressed()), this, SLOT(close()) );

    // layouting the controls
    QHBoxLayout* hboxLayout = new QHBoxLayout( _extendedOptions );
    hboxLayout->addWidget( _applyOnlyToFiles );
    hboxLayout->addWidget( _applyOnlyToFolders );

    QHBoxLayout* hboxLayoutForButtons = new QHBoxLayout();
    hboxLayoutForButtons->addStretch();
    hboxLayoutForButtons->addWidget( okButton );
    hboxLayoutForButtons->addWidget( cancelButton );

    QVBoxLayout* vboxLayout = new QVBoxLayout( this );
    vboxLayout->addWidget( _criteria );
    vboxLayout->addWidget( _extendedOptions );
    vboxLayout->addLayout( hboxLayoutForButtons );

    // making the default focused element
    _criteria->setFocus();

    // making the default enter-button
    okButton->setDefault( true );
}

AddRemoveSelection::~AddRemoveSelection()
{}

bool AddRemoveSelection::haveToDoIt()
{
    return _haveToDoIt;
}

bool AddRemoveSelection::extendedOtionsAvailable()
{
    return _extendedOptions->isChecked();
}

bool AddRemoveSelection::haveToApplyOnlyToFiles()
{
    return _applyOnlyToFiles->isChecked();
}

QString AddRemoveSelection::getCriteria()
{
    return _criteria->text();
}

void AddRemoveSelection::okClicked()
{
    _haveToDoIt = true;
    close();
}
