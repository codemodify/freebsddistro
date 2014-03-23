
#include "Tester.h"

Tester::Tester() :
    QWidget()
{
/*
    QLabel*         targetFolderLabel   = new QLabel( "Where: " );
    QLineEdit*      targetFolder        = new QLineEdit();
    QHBoxLayout*    targetFolderLayout  = new QHBoxLayout();
                    targetFolderLayout->addWidget( targetFolderLabel );
                    targetFolderLayout->addWidget( targetFolder      );

    QLabel*         nameFilterLabel     = new QLabel( "What: " );
    QLineEdit*      nameFilter          = new QLineEdit();
    QHBoxLayout*    nameFilterLayout    = new QHBoxLayout();
                    nameFilterLayout->addWidget( nameFilterLabel );
                    nameFilterLayout->addWidget( nameFilter      );

    QLabel*         contentFilterLabel  = new QLabel( "Content: " );
    QLineEdit*      contentFilter       = new QLineEdit();
    QHBoxLayout*    contentFilterLayout = new QHBoxLayout();
                    contentFilterLayout->addWidget( contentFilterLabel );
                    contentFilterLayout->addWidget( contentFilter      );

    QPushButton*    search              = new QPushButton( "search" );
    QPushButton*    cancel              = new QPushButton( "cancel" );
    QHBoxLayout*    buttonsLayout       = new QHBoxLayout();
                    buttonsLayout->addWidget( search );
                    buttonsLayout->addWidget( cancel );

    QDialog*        searchDialog        = new QDialog();

    // add controls to the visible area
    QVBoxLayout*    layout = new QVBoxLayout( searchDialog  );
                    layout->addLayout( targetFolderLayout   );
                    layout->addLayout( nameFilterLayout     );
                    layout->addLayout( contentFilterLayout  );
                    layout->addLayout( buttonsLayout        );

    connect( search, SIGNAL(pressed()), searchDialog, SLOT(accept()) );
    connect( cancel, SIGNAL(pressed()), searchDialog, SLOT(reject()) );

    searchDialog->exec();

    // check if it still was requested to search for items
    if( QDialog::Rejected == searchDialog->result() )
        return;

    FileSearch* fileSearch = new FileSearch();

    FileSearchUi*   fileSearchUi = new FileSearchUi( fileSearch );
                    fileSearchUi->show();

    fileSearch->findItems( targetFolder->text(), nameFilter->text(), contentFilter->text() );
    */
}

Tester::~Tester()
{
//    delete _fileSearch;
//    delete _fileSearchUi;
}

/*
void Tester::slot_showManager()
{
    _fileSystemOperationsUi->show();
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
void Tester::slot_createFile()
{
    _fileSystemOperations->createFile( _source->text() );
}

void Tester::slot_copyFile()
{
    _fileSystemOperations->copyFile( _source->text(), _destination->text() );
}

void Tester::slot_removeFile()
{
    _fileSystemOperations->removeFile( _source->text() );
}

void Tester::slot_renameFile()
{
    _fileSystemOperations->renameFile( _source->text(), _destination->text() );
}

void Tester::slot_moveFile()
{
    _fileSystemOperations->moveFile( _source->text(), _destination->text() );
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
void Tester::slot_createFolder()
{
    _fileSystemOperations->createFolder( _source->text() );
}

void Tester::slot_copyFolder()
{
    _fileSystemOperations->copyFolder( _source->text(), _destination->text() );
}

void Tester::slot_removeFolder()
{
    _fileSystemOperations->removeFolder( _source->text() );
}

void Tester::slot_renameFolder()
{
    _fileSystemOperations->renameFolder( _source->text(), _destination->text() );
}

void Tester::slot_moveFolder()
{
    _fileSystemOperations->moveFolder( _source->text(), _destination->text() );
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
void Tester::slot_copyObjectsToFolder(){}
void Tester::slot_moveObjectsToFolder(){}
void Tester::slot_removeObjects(){}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
void Tester::slot_createSymbolicLink(){}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
void Tester::slot_markObjectsForCopy()
{
    QStringList list;
                list << _source->text();

    _fileSystemOperations->markObjectsForCopy( list );
}

void Tester::slot_markObjectsForCut()
{
    QStringList list;
                list << _source->text();

    _fileSystemOperations->markObjectsForCut( list );
}
void Tester::slot_pasteObjects()
{
    _fileSystemOperations->pasteObjects( _destination->text() );
}
*/

