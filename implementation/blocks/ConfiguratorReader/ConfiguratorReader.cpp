
// Qt headers
#include <QtCore/QFile>
#include <QtCore/QCoreApplication>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>

// local headers
#include "ConfiguratorReader.h"


ConfiguratorReader::ConfiguratorReader( QString configurationFileName )
{
    if( configurationFileName.isEmpty() )
        configurationFileName = getAppConfigurationFile();

	// trying to access the file
	QFile file( configurationFileName );
	if( true == file.open(QIODevice::ReadOnly) )
	{
	    _configurationFile.setContent( &file );
	    file.close();
    }
}

ConfiguratorReader::~ConfiguratorReader()
{}

QString ConfiguratorReader::getAppConfigurationFile()
{
    return QCoreApplication::applicationFilePath() + ".configurator";
}

QFont ConfiguratorReader::getFont( QString pathToObject )
{
	QDomNode control = getControlNodeFromPath( pathToObject );
	if( control.isNull() )
		return QString();
	
	QDomNamedNodeMap controlAttributes = control.attributes();

    QFont   font;
           	font.fromString( controlAttributes.namedItem("value").nodeValue() );

	return font;
}
	
QColor ConfiguratorReader::getColor( QString pathToObject  )
{
	QDomNode control = getControlNodeFromPath( pathToObject );
	if( control.isNull() )
		return QString();
	
	QDomNamedNodeMap controlAttributes = control.attributes();

    QColor  color;
           	color.setNamedColor( controlAttributes.namedItem("value").nodeValue() );

	return color;
}
	
QString ConfiguratorReader::getFileSystemPath( QString pathToObject )
{
	QDomNode control = getControlNodeFromPath( pathToObject );
	if( control.isNull() )
		return QString();

	QDomNamedNodeMap controlAttributes = control.attributes();

	return controlAttributes.namedItem("value").nodeValue();
}
	
QString ConfiguratorReader::getCheckedRadioBox( QString pathToObject )
{
	QDomNode control = getControlNodeFromPath( pathToObject );
	if( control.isNull() )
		return QString();

	QDomNode radio = control.firstChild();
	while( false == radio.isNull() )
	{
		QDomNamedNodeMap radioAttributes = radio.attributes();
		if( "true" == radioAttributes.namedItem("value").nodeValue() )
			return radioAttributes.namedItem("name").nodeValue();

		radio = radio.nextSibling();
	}

	return QString();
}
	
QStringList ConfiguratorReader::getCheckedCheckBoxes( QString pathToObject )
{
	// getting the control-node
	QDomNode control = getControlNodeFromPath( pathToObject );
	if( control.isNull() )
		return QStringList();
		
	QStringList checkBoxesNames;
		
	QDomNode check = control.firstChild();
	while( false == check.isNull() )
	{
		QDomNamedNodeMap checkAttributes = check.attributes();
		if( "true" == checkAttributes.namedItem("value").nodeValue() )
			checkBoxesNames.append( checkAttributes.namedItem("name").nodeValue() );
			
		check = check.nextSibling();
	}
	
	// returning the result	
	return checkBoxesNames;
	
}

QString ConfiguratorReader::getLineInput( QString pathToObject )
{
	QDomNode control = getControlNodeFromPath( pathToObject );
	if( control.isNull() )
		return QString();

	QDomNamedNodeMap controlAttributes = control.attributes();

	return controlAttributes.namedItem("value").nodeValue();
}

QStringList ConfiguratorReader::getPlugins( QString pathToObject )
{
    QDomNode control = getControlNodeFromPath( pathToObject );
    if( control.isNull() )
        return QStringList();
    
    QDomNamedNodeMap controlAttributes = control.attributes();

    QString value = controlAttributes.namedItem("value").nodeValue();

    return value.split(";");
}

QDomNode ConfiguratorReader::getControlNodeFromPath( QString pathToObject )
{
	QStringList nodeNames = pathToObject.split( "/" );
	
	QString sectionName     = nodeNames.value( 0 );
	QString sectionItemName = nodeNames.value( 1 );
	QString controlName     = nodeNames.value( 2 );

	// searching the corresponding node in DOM representation of the configuration file
	QDomNode section = _configurationFile.documentElement().firstChild();
	while( false == section.isNull() )
	{
		QDomNamedNodeMap sectionAttributes = section.attributes();
		if( sectionName == sectionAttributes.namedItem("name").nodeValue() )
			break;

		section = section.nextSibling();
	}
	if( section.isNull() )
		return QDomNode();

	QDomNode sectionItem = section.firstChild();
	while( false == sectionItem.isNull() )
	{
		QDomNamedNodeMap sectionItemAttributes = sectionItem.attributes();
		if( sectionItemName == sectionItemAttributes.namedItem("name").nodeValue() )
			break;

		sectionItem = sectionItem.nextSibling();
	}
	if( true == sectionItem.isNull() )
		return QDomNode();
		
	QDomNode control = sectionItem.firstChild();	
	while( false == control.isNull() )
	{
        QDomNamedNodeMap controlAttributes = control.attributes();
		if( controlName == controlAttributes.namedItem("name").nodeValue() )
			break;
        
		control = control.nextSibling();
	}
	if( true == control.isNull() )
		return QDomNode();

	return control;
}

