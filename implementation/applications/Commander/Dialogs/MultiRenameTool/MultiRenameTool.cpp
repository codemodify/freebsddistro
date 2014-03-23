
// Qt headers
#include <QtCore/QFileInfo>
#include <QtGui/QPushButton>
#include <QtGui/QGroupBox>
#include <QtGui/QRadioButton>
#include <QtGui/QLineEdit>
#include <QtGui/QSpinBox>
#include <QtGui/QListWidget>
#include <QtGui/QLabel>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QListWidgetItem>

// blocks
#include <FileSystemOperations/FileSystemOperations.h>

// local
#include "MultiRenameTool.h"

MultiRenameTool::MultiRenameTool( QStringList& itemsToRename ) :
    QWidget(),
    _itemsToRename( itemsToRename )
{
    setWindowTitle( "Multi-Rename Tool" );

    // name
    _namePre              = new QLineEdit();

    _nameRemoveStartIndex = new QSpinBox();
    _nameRemoveEndIndex   = new QSpinBox();
    _nameRemoveGroup      = new QGroupBox( "Remove" );
    _nameRemoveGroup->setCheckable( true );
    _nameRemoveGroup->setChecked( false );
    QVBoxLayout*    nameVBoxLayout1 = new QVBoxLayout( _nameRemoveGroup    );
                    nameVBoxLayout1->addWidget( _nameRemoveStartIndex      );
                    nameVBoxLayout1->addWidget( _nameRemoveEndIndex        );

    _nameIndexStartIndex    = new QSpinBox();
    _nameIndexGroup         = new QGroupBox( "index" );
    _nameIndexGroup->setCheckable( true );
    _nameIndexGroup->setChecked( false );
    QVBoxLayout*    nameVBoxLayout2 = new QVBoxLayout( _nameIndexGroup    );
                    nameVBoxLayout2->addWidget( _nameIndexStartIndex      );

    _nameLowerCase = new QRadioButton( "toLower" );
    _nameUpperCase = new QRadioButton( "toUpper" );
    _nameCaseGroup = new QGroupBox( "case" );
    _nameCaseGroup->setCheckable( true );
    _nameCaseGroup->setChecked( false ); 
    QVBoxLayout*    nameVBoxLayout3 = new QVBoxLayout( _nameCaseGroup );
                    nameVBoxLayout3->addWidget( _nameLowerCase );
                    nameVBoxLayout3->addWidget( _nameUpperCase );

    QHBoxLayout*    nameHBoxLayout4 = new QHBoxLayout();
                    nameHBoxLayout4->addWidget( _nameRemoveGroup );
                    nameHBoxLayout4->addWidget( _nameIndexGroup    );
                    nameHBoxLayout4->addWidget( _nameCaseGroup    );

    _namePost                = new QLineEdit();

    _nameGroup               = new QGroupBox( "Touch Name" );
    _nameGroup->setCheckable( true );
    _nameGroup->setChecked( false );
    QGridLayout*    nameGridLayout = new QGridLayout( _nameGroup );
                    nameGridLayout->addWidget( new QLabel("pre")   , 0, 0, Qt::AlignHCenter );
                    nameGridLayout->addWidget( _namePre            , 1, 0, Qt::AlignHCenter );
                    nameGridLayout->addWidget( new QLabel("name")  , 0, 1, Qt::AlignHCenter );
                    nameGridLayout->addLayout( nameHBoxLayout4     , 1, 1, Qt::AlignHCenter );
                    nameGridLayout->addWidget( new QLabel("post")  , 0, 2, Qt::AlignHCenter );
                    nameGridLayout->addWidget( _namePost           , 1, 2, Qt::AlignHCenter );


    // suffix
    _suffixPre              = new QLineEdit();

    _suffixRemoveStartIndex = new QSpinBox();
    _suffixRemoveEndIndex   = new QSpinBox();
    _suffixRemoveGroup      = new QGroupBox( "Remove" );
    _suffixRemoveGroup->setCheckable( true );
    _suffixRemoveGroup->setChecked( false );
    QVBoxLayout*    suffixVBoxLayout1 = new QVBoxLayout( _suffixRemoveGroup    );
                    suffixVBoxLayout1->addWidget( _suffixRemoveStartIndex      );
                    suffixVBoxLayout1->addWidget( _suffixRemoveEndIndex        );

    _suffixIndexStartIndex    = new QSpinBox();
    _suffixIndexGroup         = new QGroupBox( "index" );
    _suffixIndexGroup->setCheckable( true );
    _suffixIndexGroup->setChecked( false );
    QVBoxLayout*    suffixVBoxLayout2 = new QVBoxLayout( _suffixIndexGroup    );
                    suffixVBoxLayout2->addWidget( _suffixIndexStartIndex      );

    _suffixLowerCase = new QRadioButton( "toLower" );
    _suffixUpperCase = new QRadioButton( "toUpper" );
    _suffixCaseGroup = new QGroupBox( "case" );
    _suffixCaseGroup->setCheckable( true );
    _suffixCaseGroup->setChecked( false );    
    QVBoxLayout*    suffixVBoxLayout3 = new QVBoxLayout( _suffixCaseGroup );
                    suffixVBoxLayout3->addWidget( _suffixLowerCase );
                    suffixVBoxLayout3->addWidget( _suffixUpperCase );

    QHBoxLayout*    suffixHBoxLayout4 = new QHBoxLayout();
                    suffixHBoxLayout4->addWidget( _suffixRemoveGroup );
                    suffixHBoxLayout4->addWidget( _suffixIndexGroup  );
                    suffixHBoxLayout4->addWidget( _suffixCaseGroup   );

    _suffixPost                = new QLineEdit();

    _suffixGroup               = new QGroupBox( "Touch Suffix" );
    _suffixGroup->setCheckable( true );
    _suffixGroup->setChecked( false );
    QGridLayout*    suffixGridLayout = new QGridLayout( _suffixGroup );
                    suffixGridLayout->addWidget( new QLabel("pre")   , 0, 0, Qt::AlignHCenter );
                    suffixGridLayout->addWidget( _suffixPre            , 1, 0, Qt::AlignHCenter );
                    suffixGridLayout->addWidget( new QLabel("suffix")  , 0, 1, Qt::AlignHCenter );
                    suffixGridLayout->addLayout( suffixHBoxLayout4     , 1, 1, Qt::AlignHCenter );
                    suffixGridLayout->addWidget( new QLabel("post")  , 0, 2, Qt::AlignHCenter );
                    suffixGridLayout->addWidget( _suffixPost           , 1, 2, Qt::AlignHCenter );

    // name and sufix
    QHBoxLayout*    nameAndSuffix = new QHBoxLayout();
                    nameAndSuffix->addWidget( _nameGroup      );
                    nameAndSuffix->addWidget( _suffixGroup        );    


    // items
    _sourceItems    = new QListWidget();
    _previewItems   = new QListWidget();
    QGridLayout*    itemsLayout = new QGridLayout();
                    itemsLayout->addWidget( new QLabel("SourceItems")    , 0, 0, Qt::AlignLeft    );
                    itemsLayout->addWidget( _sourceItems                 , 1, 0                   );
                    itemsLayout->addWidget( new QLabel("Preview")        , 0, 1, Qt::AlignLeft    );
                    itemsLayout->addWidget( _previewItems                , 1, 1                   );


    // buttons
    _apply       = new QPushButton( "Apply"        );
    _cancel      = new QPushButton( "Cancel"    );
    _preview     = new QPushButton( "Preview"    );
    QHBoxLayout*    buttonsLayout = new QHBoxLayout();
                    buttonsLayout->addWidget( _apply );
                    buttonsLayout->addWidget( _cancel );
                    buttonsLayout->addStretch();
                    buttonsLayout->addWidget( _preview );

    // bind all together
    QVBoxLayout*    thisLayout = new QVBoxLayout( this );
                    thisLayout->addLayout( nameAndSuffix    );
                    thisLayout->addLayout( itemsLayout      );
                    thisLayout->addLayout( buttonsLayout    );

    // populate the source items
    foreach( QString item, _itemsToRename )
        _sourceItems->addItem( QFileInfo(item).fileName() );

    // the connect part
    connect( _apply,      SIGNAL(pressed()), this, SLOT(apply())      );
    connect( _cancel,     SIGNAL(pressed()), this, SLOT(close())      );
    connect( _preview,    SIGNAL(pressed()), this, SLOT(preview())    );
}

MultiRenameTool::~MultiRenameTool()
{}

void MultiRenameTool::closeEvent( QCloseEvent* event )
{
    deleteLater();

    QWidget::closeEvent( event );
}

void MultiRenameTool::apply()
{
    preview();

    FileSystemOperations*   fileSystemOperations = new FileSystemOperations();
                            fileSystemOperations->setDestroyOnFinish( true );

    for( int i=0; i < _itemsToRename.count(); i++ )
        fileSystemOperations->renameFile( _itemsToRename.value(i), _previewItems->item(i)->text() );
}

void MultiRenameTool::preview()
{
    _previewItems->clear();

    for( int i=0; i < _itemsToRename.count(); i++ )
    {
        QString fileName    = QFileInfo( _itemsToRename.value(i) ).completeBaseName();
        QString suffix      = QFileInfo( _itemsToRename.value(i) ).suffix();

        // name
        if( _nameGroup->isChecked() )
        {
            if( _nameRemoveGroup->isChecked() )
                fileName.remove( _nameRemoveStartIndex->value(), _nameRemoveEndIndex->value() );

            if( _nameIndexGroup->isChecked() )
                fileName = QString("%1%2").arg(fileName).arg( _nameIndexStartIndex->value()+i );

            if( _nameCaseGroup->isChecked() )
                fileName = _nameLowerCase->isChecked() ? fileName.toLower() : fileName.toUpper();

            fileName = QString("%1%2%3").arg(_namePre->text()).arg(fileName).arg(_namePost->text());
        }

        // suffix
        if( _suffixGroup->isChecked() )
        {
            if( _suffixRemoveGroup->isChecked() )
                suffix.remove( _suffixRemoveStartIndex->value(), _suffixRemoveEndIndex->value() );

            if( _suffixIndexGroup->isChecked() )
                suffix = QString("%1%2").arg(fileName).arg( _suffixIndexStartIndex->value()+i );

            if( _suffixCaseGroup->isChecked() )
                suffix = _suffixLowerCase->isChecked() ? suffix.toLower() : suffix.toUpper();

            suffix = QString("%1%2%3").arg(_suffixPre->text()).arg(suffix).arg(_suffixPost->text());
        }

        // add to the preview list
        _previewItems->addItem( fileName + (suffix.isEmpty() ? suffix : ("." + suffix)) );
    }
}
