
#include "Tester.h"

Tester::Tester() :
    QWidget()
{
    // init controls
    QPushButton* showManager         = new QPushButton( "show operations"       );

    _source                          = new QLineEdit();
    _destination                     = new QLineEdit();

    QPushButton* createFile          = new QPushButton( "createFile"            );
    QPushButton* copyFile            = new QPushButton( "copyFile"              );
    QPushButton* removeFile          = new QPushButton( "removeFile"            );
    QPushButton* renameFile          = new QPushButton( "renameFile"            );
    QPushButton* moveFile            = new QPushButton( "moveFile"              );

    QPushButton* createFolder        = new QPushButton( "createFolder"          );
    QPushButton* copyFolder          = new QPushButton( "copyFolder"            );
    QPushButton* removeFolder        = new QPushButton( "removeFolder"          );
    QPushButton* renameFolder        = new QPushButton( "renameFolder"          );
    QPushButton* moveFolder          = new QPushButton( "moveFolder"            );

    QPushButton* copyObjectsToFolder = new QPushButton( "copyObjectsToFolder"   );
    QPushButton* moveObjectsToFolder = new QPushButton( "moveObjectsToFolder"   );
    QPushButton* removeObjects       = new QPushButton( "removeObjects"         );

    QPushButton* createSymbolicLink  = new QPushButton( "createSymbolicLink"    );

    QPushButton* markObjectsForCopy  = new QPushButton( "markObjectsForCopy"    );
    QPushButton* markObjectsForCut   = new QPushButton( "markObjectsForCut"     );
    QPushButton* pasteObjects        = new QPushButton( "pasteObjects"          );

    // add controls to the visible area
    QVBoxLayout*    layout = new QVBoxLayout( this );
                    layout->addWidget   ( showManager           );
                    layout->addStretch  ();
                    layout->addWidget   ( _source               );
                    layout->addWidget   ( _destination          );
                    layout->addStretch  ();
                    layout->addWidget   ( createFile            );
                    layout->addWidget   ( copyFile              );
                    layout->addWidget   ( removeFile            );
                    layout->addWidget   ( renameFile            );
                    layout->addWidget   ( moveFile              );
                    layout->addStretch  ();
                    layout->addWidget   ( createFolder          );
                    layout->addWidget   ( copyFolder            );
                    layout->addWidget   ( removeFolder          );
                    layout->addWidget   ( renameFolder          );
                    layout->addWidget   ( moveFolder            );
                    layout->addStretch  ();
                    layout->addWidget   ( copyObjectsToFolder   );
                    layout->addWidget   ( moveObjectsToFolder   );
                    layout->addWidget   ( removeObjects         );
                    layout->addStretch  ();
                    layout->addWidget   ( createSymbolicLink    );
                    layout->addStretch  ();
                    layout->addWidget   ( markObjectsForCopy    );
                    layout->addWidget   ( markObjectsForCut     );
                    layout->addWidget   ( pasteObjects          );

    // subscribe for the press events
    connect( showManager,           SIGNAL(pressed()), this, SLOT(slot_showManager())           );

    connect( createFile,            SIGNAL(pressed()), this, SLOT(slot_createFile())            );
    connect( copyFile,              SIGNAL(pressed()), this, SLOT(slot_copyFile())              );
    connect( removeFile,            SIGNAL(pressed()), this, SLOT(slot_removeFile())            );
    connect( renameFile,            SIGNAL(pressed()), this, SLOT(slot_renameFile())            );
    connect( moveFile,              SIGNAL(pressed()), this, SLOT(slot_moveFile())              );

    connect( createFolder,          SIGNAL(pressed()), this, SLOT(slot_createFolder())          );
    connect( copyFolder,            SIGNAL(pressed()), this, SLOT(slot_copyFolder())            );
    connect( removeFolder,          SIGNAL(pressed()), this, SLOT(slot_removeFolder())          );
    connect( renameFolder,          SIGNAL(pressed()), this, SLOT(slot_renameFolder())          );
    connect( moveFolder,            SIGNAL(pressed()), this, SLOT(slot_moveFolder())            );

    connect( copyObjectsToFolder,   SIGNAL(pressed()), this, SLOT(slot_copyObjectsToFolder())   );
    connect( moveObjectsToFolder,   SIGNAL(pressed()), this, SLOT(slot_moveObjectsToFolder())   );
    connect( removeObjects,         SIGNAL(pressed()), this, SLOT(slot_removeObjects())         );

    connect( createSymbolicLink,    SIGNAL(pressed()), this, SLOT(slot_createSymbolicLink())    );

    connect( markObjectsForCopy,    SIGNAL(pressed()), this, SLOT(slot_markObjectsForCopy())    );
    connect( markObjectsForCut,     SIGNAL(pressed()), this, SLOT(slot_markObjectsForCut())     );
    connect( pasteObjects,          SIGNAL(pressed()), this, SLOT(slot_pasteObjects())          );

    // create the test objects
    _fileSystemOperations = new FileSystemOperations();
    _fileSystemOperations->setMaxThreadsLimit( 1 );

    _fileSystemOperationsUi = new FileSystemOperationsUi( _fileSystemOperations );
}

Tester::~Tester()
{
    delete _fileSystemOperations;
    delete _fileSystemOperationsUi;
}

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
