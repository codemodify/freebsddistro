
// Qt hreaders
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QString>
#include <QtXml/QDomNode>
#include <QtGui/QListWidget>
#include <QtGui/QListWidgetItem>
#include <QtGui/QGroupBox>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QFontDialog>
#include <QtGui/QColorDialog>
#include <QtGui/QFileDialog>
#include <QtGui/QRadioButton>
#include <QtGui/QCheckBox>
#include <QtGui/QTextBrowser>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QToolBox>
#include <QtGui/QFrame>
#include <QtGui/QPushButton>
#include <QtGui/QApplication>
#include <QtWebKit/QWebView>

// blocks
#include <Pluginer/PluginManager.h>
#include <PluginerUi/PluginerUi.h>

// local headers
#include "Configurator.h"
#include "Helper.h"

Configurator::Configurator( QStringList parameter ) :
	QDialog(),
    _pluginManager( 0 ),
    _localPluginManager( true )
{
    _configurationFileName  = parameter.value( 0 );
    _startSectionName       = parameter.value( 1 );
    _startSectionItemName   = parameter.value( 2 );

    constructorHelper();
}

Configurator::Configurator( QStringList parameter, PluginManager* pluginManager ) :
    QDialog(),
    _pluginManager( pluginManager ),
    _localPluginManager( false )
{
    _configurationFileName  = parameter.value( 0 );
    _startSectionName       = parameter.value( 1 );
    _startSectionItemName   = parameter.value( 2 );

    constructorHelper();
}

Configurator::~Configurator()
{

    if( (true == _localPluginManager) && (0 != _pluginManager) )
        delete _pluginManager;
}

void Configurator::constructorHelper()
{
	_currentWigetItem = 0;

	initUi();
	initUiFromSettings();
}

QString Configurator::getAppConfigurationFile()
{
    return QApplication::applicationFilePath() + ".configurator";
}

QString Configurator::getActiveSectionItem()
{
  	QListWidget* listWidget = qobject_cast<QListWidget*>( _sections->currentWidget() );
   	if( 0 != listWidget )
   		return listWidget->objectName() +"/"+ listWidget->item( listWidget->currentRow() )->data( Qt::UserRole ).toString();

	return QString();
}

void Configurator::closeEvent( QCloseEvent* event )
{
    deleteLater();

    QDialog::closeEvent( event );
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// init stuff
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void Configurator::initUi()
{
	// build the user interface
	_sections			= new QToolBox();
	_sections->setMaximumWidth( 200 );

	_configOptions	= new QFrame();
	_configOptions->setMinimumWidth( 600 );
	
	_save			= new QPushButton( "OK" );
	_close			= new QPushButton( "Close" );

	QHBoxLayout*	hBoxLayout1 = new QHBoxLayout();
				hBoxLayout1->addWidget( _sections		);
				hBoxLayout1->addWidget( _configOptions	);				    

	QHBoxLayout*	hBoxLayout2 = new QHBoxLayout();
				hBoxLayout2->addSpacerItem( new QSpacerItem(3, 3, QSizePolicy::Expanding, QSizePolicy::Maximum) );
				hBoxLayout2->addWidget( _save	);
				hBoxLayout2->addWidget( _close	);

	QVBoxLayout*	vBoxLayout = new QVBoxLayout( this );
				    vBoxLayout->addLayout( hBoxLayout1 );
				    vBoxLayout->addLayout( hBoxLayout2 );

	// set handlers for UI
	connect( _sections, SIGNAL(currentChanged(int)), this, SLOT(currentChanged(int)) );

	connect( _save,	SIGNAL(clicked()), this, SLOT(saveSettings())	);
	connect( _close,	SIGNAL(clicked()), this, SLOT(close())		);
	
	setMinimumHeight( 500 );
	setMinimumWidth( 800 );
}

void Configurator::initUiFromSettings()
{
	// load section from the file
	if( false == loadSettingsFile() )
		return;

    // focus the right section
    int sectionIndex = 0;
    if( false == _startSectionName.isEmpty() )
    {
        for( ; sectionIndex < _sections->count(); sectionIndex++ )
        {
            QString sectionName = _sections->widget(sectionIndex)->objectName();
            if( _startSectionName == sectionName )
	            break;
	    }
	}
	_sections->setCurrentIndex( sectionIndex );

    // focus the right sectionItem
  	QListWidget* listWidget = qobject_cast<QListWidget*>( _sections->currentWidget() );
   	if( 0 != listWidget )
   	{
   	    int sectionItemIndex = 0;
   	    for( ; sectionItemIndex < listWidget->count(); sectionItemIndex++ )
   	    {
   	        QString itemName = listWidget->item( sectionItemIndex )->data( Qt::UserRole ).toString();
   	        if( _startSectionItemName == itemName )
   	            break;
   	    }

        sectionItemIndex = (sectionItemIndex >= listWidget->count()) ? 0 : sectionItemIndex;
        listWidget->setCurrentRow( sectionItemIndex );
   	}
}

bool Configurator::loadSettingsFile()
{
	// check if we can read the file
	QFile configurationFile( _configurationFileName );
	if( false == configurationFile.open(QIODevice::ReadOnly) )
		return false;
	
	// loading settings file into memory
	if( false == _domDocument.setContent(&configurationFile) )
	{
		configurationFile.close();
		return false;
	}
	configurationFile.close();

	// validating the root node
	QDomNode configurator = _domDocument.documentElement();
	if( configurator.isNull() )
		return false;

	if( configurator.nodeName() != c_configuratorNodeName )
		return false;

	if( false == configurator.hasChildNodes() )
		return false;

	// set title text
	QDomNamedNodeMap configuratorAttributes = configurator.attributes();
	setWindowTitle( configuratorAttributes.namedItem(c_attributeTitleText).nodeValue() );	
	
	// validating and adding the sections nodes
	QDomNode section = configurator.firstChild();
	while( false == section.isNull() )
	{
	    if( QDomNode::CommentNode != section.nodeType() )
		if( false == loadSection(section) )
			return false;

		section = section.nextSibling();		
	}

	return true;
}

bool Configurator::loadSection( QDomNode section )
{
	// make some checks
	if( false == section.hasChildNodes() )
		return false;

	if( section.nodeName() != c_sectionNodeName )
		return false;

	QDomNamedNodeMap sectionAttributes = section.attributes();
	if( false == sectionAttributes.contains(c_attributeName) )
		return false;

	if( false == sectionAttributes.contains(c_attributeText) )
		return false;

	// we have all the attribbutes for this section, now build it usign GUI controls
	QString sectionName = sectionAttributes.namedItem(c_attributeName).nodeValue();
	QString sectionText = sectionAttributes.namedItem(c_attributeText).nodeValue();
	
	// build the UI
	QListWidget*	listWidget = new QListWidget();
				    listWidget->setObjectName( sectionName );
				    listWidget->setSelectionBehavior( QAbstractItemView::SelectRows );

	connect( listWidget,    SIGNAL	(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
			 this,		    SLOT	(currentItemChanged(QListWidgetItem*,QListWidgetItem*))	);

	_sections->addItem( listWidget, sectionText );
			
	// adding items for this section
	QDomNode sectionItem = section.firstChild();
	while( false == sectionItem.isNull() )
	{
	    if( QDomNode::CommentNode != section.nodeType() )
		if( false == loadSectionItem(sectionItem,*listWidget) )
			return false;
	
		sectionItem = sectionItem.nextSibling();				
	}

	return true;
}

bool Configurator::loadSectionItem( QDomNode sectionItem, QListWidget& listWidget )
{
	if( false == sectionItem.hasChildNodes() )
		return false;

	if( sectionItem.nodeName() != c_sectionItemNodeName )
		return false;

	QDomNamedNodeMap sectionItemAttributes = sectionItem.attributes();
	if( false == sectionItemAttributes.contains(c_attributeName) )
		return false;

	if( false == sectionItemAttributes.contains(c_attributeText) )
		return false;

	// add the item to the section
	QString sectionItemName = sectionItemAttributes.namedItem(c_attributeName).nodeValue();
	QString sectionItemText = sectionItemAttributes.namedItem(c_attributeText).nodeValue();
	
	QListWidgetItem*	listWidgetItem = new QListWidgetItem();
					    listWidgetItem->setText( sectionItemText );
					    listWidgetItem->setData( Qt::UserRole, sectionItemName );

    listWidget.addItem( listWidgetItem );

	return true;
}

void Configurator::saveSettings()
{
    QListWidget* listWidget = qobject_cast<QListWidget*>( _sections->currentWidget() );
	if( 0 != listWidget )
		saveItem( listWidget->currentItem() );

	// cheking if we can open file for writing
	QFile configurationFile( _configurationFileName );
	if( false == configurationFile.open(QIODevice::WriteOnly) )
		return;

	// saving content
	QTextStream textStream( &configurationFile );

	_domDocument.save( textStream, 4 );

	configurationFile.close();

	emit configurationChanged();

	accept();
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// handlers for controls
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void Configurator::currentChanged( int index )
{
	// saving data for the previous section
	if( _configOptions->children().count() && _currentWigetItem )
	{
		saveItem( _currentWigetItem );
		_currentWigetItem->listWidget()->setCurrentRow( -1 );
	}
	
	// find the new section and set current item to first
	QListWidget* listWidget = qobject_cast<QListWidget*>( _sections->widget(index) );
	if( 0 == listWidget )
		return;

	listWidget->setCurrentRow( 0 );
}

void Configurator::currentItemChanged( QListWidgetItem* current, QListWidgetItem* previous )
{
	_currentWigetItem = current;
	
	saveItem( previous );
	loadItem( current );
}

void Configurator::loadItem( QListWidgetItem* item )
{
	if( 0 == item )
		return;
	
	// searching corresponding section
	QString     sectionName     = item->listWidget()->objectName();
	QDomNode    configurator    = _domDocument.documentElement();
	QDomNode    section         = configurator.firstChild();
	while( false == section.isNull() )
	{
		QDomNamedNodeMap sectionAttributes = section.attributes();
		if( sectionName == sectionAttributes.namedItem(c_attributeName).nodeValue() )
			break;

		section = section.nextSibling();
	}

	// having section, searching for the sectionItem
	QString     sectionItemName = item->data( Qt::UserRole ).toString();
	QDomNode    sectionItem     = section.firstChild();
	while( false == sectionItem.isNull() )
	{
		QDomNamedNodeMap sectionItemAttributes = sectionItem.attributes();

		if( QDomNode::CommentNode != section.nodeType() )
		if( sectionItemName == sectionItemAttributes.namedItem(c_attributeName).nodeValue() )
			break;

		sectionItem = sectionItem.nextSibling();
	}

	// destroying all previous controls from the frame
	QObjectList children = _configOptions->children();
	foreach( QObject* child, children )
		delete child;	

	// vertical-layout all the frame's objects
	QVBoxLayout* verticalLayout = new QVBoxLayout( _configOptions );

	// at this step we have everything to start load data from DOM
	QDomNode control = sectionItem.firstChild();
	while( false == control.isNull() )
	{
		// note: not all the nodes met will have these attributes, speciffic nodes will have some of these
		QDomNamedNodeMap			  controlAttributes = control.attributes();
		QString controlName		= controlAttributes.namedItem( c_attributeName  ).nodeValue();
		QString controlText		= controlAttributes.namedItem( c_attributeText  ).nodeValue();
		QString controlType		= controlAttributes.namedItem( c_attributeType  ).nodeValue();
		QString controlValue		= controlAttributes.namedItem( c_attributeValue ).nodeValue();

		switch( getControlTypeByName(controlType) )
		{
			case	eFontChooser	: verticalLayout->addWidget( createFontChooser(controlName,controlText,controlValue)					);	break;
			case	eColorChooser	: verticalLayout->addWidget( createColorChooser(controlName,controlText,controlValue)					);	break;
			case	ePathChooser	: verticalLayout->addWidget( createPathChooser(controlName,controlText,controlValue)					);	break;
			case	eRadioBoxes	: verticalLayout->addWidget( createRadioBoxes(controlName,controlText,controlValue,control.firstChild())	);	break;
			case	eCheckBoxes	: verticalLayout->addWidget( createCheckBoxes(controlName,controlText,controlValue,control.firstChild())	);	break;
			case	eTextViewer	: verticalLayout->addWidget( createTextViewer(controlName,control)									);	break;
			case	eLineInput	: verticalLayout->addWidget( createLineInput(controlName,controlText,controlValue)						);	break;
			case	ePlugins      : verticalLayout->addWidget( createPlugins(controlName,controlValue)                                      );	break;

			default:
				break;			
		}

		control = control.nextSibling();
	}
}

void Configurator::saveItem( QListWidgetItem* item )
{
	if( 0 == item )
		return;

	// searching for the corresponding group-node
    QString     sectionName		= item->listWidget()->objectName();
	QDomNode    configurator    = _domDocument.documentElement();
	QDomNode    section		    = configurator.firstChild();
	while( false == section.isNull() )
	{
		QDomNamedNodeMap sectionAttributes = section.attributes();
		
		if( QDomNode::CommentNode != section.nodeType() )
		if( sectionName == sectionAttributes.namedItem(c_attributeName).nodeValue() )
			break;

		section = section.nextSibling();
	}

	// having section, searching for the sectionItem
	QString     sectionItemName = item->text();
	QDomNode    sectionItem     = section.firstChild();
	while( false == sectionItem.isNull() )
	{
		QDomNamedNodeMap sectionItemAttributes = sectionItem.attributes();
		
		if( QDomNode::CommentNode != section.nodeType() )
		if( sectionItemName == sectionItemAttributes.namedItem(c_attributeText).nodeValue() )
			break;

		sectionItem = sectionItem.nextSibling();
	}

    // at this step we have everything to start save data back to DOM
    QDomNode control = sectionItem.firstChild();
    while( false == control.isNull() )
    {
        QDomNamedNodeMap          controlAttributes = control.attributes();
        QString controlName     = controlAttributes.namedItem(c_attributeName).nodeValue();
        QString controlText     = controlAttributes.namedItem(c_attributeText).nodeValue();
        QString controlType     = controlAttributes.namedItem(c_attributeType).nodeValue();
        QString controlValue    = controlAttributes.namedItem(c_attributeValue).nodeValue();
        
        switch( getControlTypeByName(controlType) )
        {
            case eFontChooser	: saveFontChooser   ( controlName, controlAttributes    ); break;
            case eColorChooser  : saveColorChooser  ( controlName, controlAttributes    ); break;
            case ePathChooser	: savePathChooser   ( controlName, controlAttributes    ); break;
            case eRadioBoxes	: saveRadioBoxes    ( controlName, control.firstChild() ); break;
            case eCheckBoxes	: saveCheckBoxes    ( controlName, control.firstChild() ); break;
            case eLineInput     : saveLineInput     ( controlName, controlAttributes    ); break;
            case ePlugins       : savePlugins       ( controlName, controlAttributes    ); break;
            
            default:
                break;			
        }
        
        // moving to the next control
        control = control.nextSibling();
    }
}

void Configurator::fontChoose()
{
	bool ok;
	QFont font = QFontDialog::getFont( &ok );

	// if user pressed ok-button, then remember choosed font
	if( ok )
	{
		QObjectList children = sender()->parent()->children();

		// searching for the edit-control
		foreach( QObject* child, children )
		{
			if(QString("QLineEdit") == child->metaObject()->className())
			{
				(qobject_cast<QLineEdit*>(child))->setText( font.toString() );
				(qobject_cast<QLineEdit*>(child))->setFont( font );
				break;
			}
		}
	}
}

void Configurator::colorChoose()
{
    QColor color = QColorDialog::getColor();                     
	
	// if user pressed ok-button, then remember choosed font
	if( color.isValid() )
	{
		QObjectList children = sender()->parent()->children();
		
		// searching for the edit-control
		foreach( QObject* child, children )
		{
			if(QString("QLineEdit") == child->metaObject()->className())
			{
				(qobject_cast<QLineEdit*>(child))->setText( color.name() );
				(qobject_cast<QLineEdit*>(child))->setPalette( QPalette(color) );
				break;
			}
		}
	}
}

void Configurator::pathChoose()
{
	QString directory = QFileDialog::getExistingDirectory();
	if( !directory.isEmpty() )
	{
		QObjectList children = sender()->parent()->children();
		
		// searching for the edit-control
		foreach( QObject* child, children )
		{
			if(QString("QLineEdit") == child->metaObject()->className())
			{
				(qobject_cast<QLineEdit*>(child))->setText( directory );
				break;
			}
		}
    }
}



QWidget* Configurator::createFontChooser( QString controlName, QString controlText, QString controlValue )
{
	QGroupBox*	groupBox = new QGroupBox();
				groupBox->setTitle( controlText );

	QFont		font;
				font.fromString( controlValue );

	QLineEdit*	lineEdit = new QLineEdit();
				lineEdit->setObjectName( controlName );
				lineEdit->setText( controlValue );
				lineEdit->setReadOnly( true );
				lineEdit->setFont( font );
	
	QPushButton*	button = new QPushButton( "..." );
	connect( button, SIGNAL(released()), this, SLOT(fontChoose()) );

	// horizontal-layout this
	QHBoxLayout*	hBoxLayout = new QHBoxLayout( groupBox );
				hBoxLayout->addWidget( lineEdit );
				hBoxLayout->addWidget( button );

	return groupBox;
}

QWidget* Configurator::createColorChooser( QString controlName, QString controlText, QString controlValue )
{
	QGroupBox*	groupBox = new QGroupBox();
				groupBox->setTitle( controlText );

	QColor		color;
				color.setNamedColor( controlValue );

	QLineEdit*	lineEdit = new QLineEdit();
				lineEdit->setObjectName( controlName );
				lineEdit->setText( controlValue );
				lineEdit->setReadOnly( true );
				lineEdit->setPalette( QPalette(color) );

	QPushButton*	button = new QPushButton( "..." );
	connect( button, SIGNAL(clicked()), this, SLOT(colorChoose()) );

	// horizontal-layout this
	QHBoxLayout*	hBoxLayout = new QHBoxLayout( groupBox );
				hBoxLayout->addWidget( lineEdit );
				hBoxLayout->addWidget( button );

	return groupBox;
}

QWidget* Configurator::createPathChooser( QString controlName, QString controlText, QString controlValue )
{
	QGroupBox*	groupBox = new QGroupBox();
				groupBox->setTitle( controlText );

	QLineEdit*	lineEdit = new QLineEdit();
				lineEdit->setObjectName( controlName );
				lineEdit->setText( controlValue );

	QPushButton* button = new QPushButton( "..." );
	connect( button, SIGNAL(clicked()), this, SLOT(pathChoose()) );

	// horizontal-layout this
	QHBoxLayout*	hBoxLayout = new QHBoxLayout( groupBox );
				hBoxLayout->addWidget( lineEdit );
				hBoxLayout->addWidget( button );

	return groupBox;
}

QWidget* Configurator::createRadioBoxes( QString controlName, QString controlText, QString controlValue, QDomNode radioNode )
{
    QGroupBox*  groupBox = new QGroupBox();
                groupBox->setObjectName( controlName );
                groupBox->setTitle( controlText );

	QVBoxLayout* vBoxLayout = new QVBoxLayout( groupBox );
	while( false == radioNode.isNull() )
	{
		QDomNamedNodeMap radioAttributes = radioNode.attributes();
		QString radioName   = radioAttributes.namedItem(c_attributeName).nodeValue();
		QString radioText   = radioAttributes.namedItem(c_attributeText).nodeValue();
		QString radioValue  = radioAttributes.namedItem(c_attributeValue).nodeValue();

		QRadioButton*	radioButton = new QRadioButton();
					radioButton->setObjectName	( radioName 			);
					radioButton->setText		( radioText 			);
					radioButton->setChecked	( radioValue == "true" );

		vBoxLayout->addWidget( radioButton );

		radioNode = radioNode.nextSibling();
    }

	return groupBox;
}

QWidget* Configurator::createCheckBoxes( QString controlName, QString controlText, QString controlValue, QDomNode cehckBoxNode )
{
	QGroupBox*	groupBox = new QGroupBox();
				groupBox->setObjectName( controlName );
				groupBox->setTitle( controlText );
					
	// horizontal-layouting this
	QVBoxLayout* vBoxLayout = new QVBoxLayout( groupBox );
	while( false == cehckBoxNode.isNull() )
	{
		QDomNamedNodeMap radioAttributes = cehckBoxNode.attributes();
		QString checkName	= radioAttributes.namedItem(c_attributeName).nodeValue();
		QString checkText 	= radioAttributes.namedItem(c_attributeText).nodeValue();
		QString checkValue	= radioAttributes.namedItem(c_attributeValue).nodeValue();
						
		QCheckBox*	checkBox = new QCheckBox( groupBox );
					checkBox->setObjectName( checkName			);
					checkBox->setText		( checkText			);
					checkBox->setChecked	( checkValue == "true"	);

		vBoxLayout->addWidget( checkBox );
		
		cehckBoxNode = cehckBoxNode.nextSibling();		
	}

	return groupBox;
}

QWidget* Configurator::createTextViewer( QString controlName, QDomNode control )
{
    QWidget* widget = 0;

	// this is a URL node
	if( !control.attributes().namedItem(c_attributeUrl).isNull() )
	{
	    QWebView*   webView = new QWebView();
		            webView->load( QUrl(control.attributes().namedItem(c_attributeUrl).nodeValue()) );
		
		widget = webView;
	}

	// this is a palin text node
	else if( !control.attributes().namedItem(c_attributeValue).isNull() )
	{
	    QTextBrowser*	textBrowser = new QTextBrowser();
		                textBrowser->setText( control.attributes().namedItem(c_attributeValue).nodeValue() );
	    
	    widget = textBrowser;
	}

	// this is a HTML node
	else
	{
	    QTextBrowser*	textBrowser = new QTextBrowser();
		                textBrowser->setHtml( control.toElement().text() );

        widget = textBrowser;
	}

    
    widget->setObjectName( controlName );
	return  widget;
}

QWidget* Configurator::createLineInput( QString controlName, QString controlText, QString controlValue )
{
	QGroupBox*	groupBox = new QGroupBox();
				groupBox->setTitle( controlText );

	QLineEdit*	lineEdit = new QLineEdit();
				lineEdit->setObjectName( controlName );
				lineEdit->setText( controlValue );

	// horizontal-layout this
	QHBoxLayout*	hBoxLayout = new QHBoxLayout( groupBox );
				hBoxLayout->addWidget( lineEdit );

	return groupBox;
}

QWidget* Configurator::createPlugins( QString controlName, QString controlValue )
{
    if( 0 == _pluginManager )
    {
        _localPluginManager = true;
        _pluginManager = new PluginManager();
    }

    PluginerUi* pluginerUi = new PluginerUi( *_pluginManager );

    return pluginerUi;
}

void Configurator::saveFontChooser( QString controlName, QDomNamedNodeMap controlAttributes )
{
	QLineEdit* lineEdit = _configOptions->findChild<QLineEdit*>( controlName );
	if( 0 == lineEdit )
	    return;

	controlAttributes.namedItem(c_attributeValue).setNodeValue( lineEdit->text() );
}

void Configurator::saveColorChooser( QString controlName, QDomNamedNodeMap controlAttributes )
{
	QLineEdit* lineEdit = _configOptions->findChild<QLineEdit*>( controlName );
	if( 0 == lineEdit )
	    return;

	controlAttributes.namedItem(c_attributeValue).setNodeValue( lineEdit->text() );
}

void Configurator::savePathChooser( QString controlName, QDomNamedNodeMap controlAttributes )
{
	QLineEdit* lineEdit = _configOptions->findChild<QLineEdit*>( controlName );
	if( 0 == lineEdit )
	    return;

	controlAttributes.namedItem(c_attributeValue).setNodeValue( lineEdit->text() );
}

void Configurator::saveRadioBoxes( QString controlName, QDomNode radioNode )
{
	QGroupBox* groupBox = _configOptions->findChild<QGroupBox*>( controlName );
	if( 0 == groupBox )
	    return;

	while( false == radioNode.isNull() )
	{
		QDomNamedNodeMap	radioAttributes	= radioNode.attributes();
		QString			    radioName			= radioAttributes.namedItem(c_attributeName).nodeValue();
		QRadioButton*		radioButton		= groupBox->findChild<QRadioButton*>( radioName );

		radioAttributes.namedItem(c_attributeValue).setNodeValue( radioButton->isChecked() ? "true" : "false" );

		radioNode = radioNode.nextSibling();
	}
}

void Configurator::saveCheckBoxes( QString controlName, QDomNode checkBoxNode )
{
	QGroupBox* groupBox = _configOptions->findChild<QGroupBox*>( controlName );
	if( 0 == groupBox )
	    return;

	while( false == checkBoxNode.isNull() )
	{
		QDomNamedNodeMap	    checkBoxAttributes		= checkBoxNode.attributes();
		QString			    checkBoxName			= checkBoxAttributes.namedItem(c_attributeName).nodeValue();
		QCheckBox*		    checkBox				= groupBox->findChild<QCheckBox*>( checkBoxName );

		checkBoxAttributes.namedItem(c_attributeValue).setNodeValue( checkBox->isChecked() ? "true" : "false" );

		checkBoxNode = checkBoxNode.nextSibling();
	}
}

void Configurator::saveLineInput( QString controlName, QDomNamedNodeMap controlAttributes )
{
    QLineEdit* lineEdit = _configOptions->findChild<QLineEdit*>( controlName );
    if( 0 == lineEdit )
        return;

    controlAttributes.namedItem(c_attributeValue).setNodeValue( lineEdit->text() );
}

void Configurator::savePlugins( QString controlName, QDomNamedNodeMap controlAttributes )
{
    QString pluginNames;

    QStringList loadedPlugins;
    _pluginManager->getLoadedPlugins( loadedPlugins );

    foreach( QString pluginPath, loadedPlugins )
    {
        pluginNames += QString( ";%1" ).arg( pluginPath );
    }
    pluginNames.remove( 0, 1 ); // remove the first ';'

    controlAttributes.namedItem(c_attributeValue).setNodeValue( pluginNames );
}
