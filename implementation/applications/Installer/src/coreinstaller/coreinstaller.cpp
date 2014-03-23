
// qt headers
#include <QtCore/QMap>
#include <QtCore/QFile>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtCore/QStringList>

// local headers
#include "coreinstaller.h"

// this is a variable that is pointer to the list of all the environement variables defined on the system
extern char** environ;

CoreInstaller::CoreInstaller() :
	QObject(),
	_weAreInDebugMode(false)
{}

CoreInstaller::~CoreInstaller()
{}

bool CoreInstaller::setInstallFile( QString installFilePath )
{
	printf( "dsddddddddddddddd" );

	QFile file( installFilePath );
	if( false == file.open(QFile::ReadOnly) )
	{
		qDebug( QString("failed to open the file %1").arg(installFilePath).toAscii().data() );
		return false;
	}
	
	QDomDocument installFileAsXml;
	if( false == installFileAsXml.setContent(&file) )
	{
		file.close();
		qDebug( QString("file is not XML %1").arg(installFilePath).toAscii().data() );
		return false;
	}
	file.close();
	
	readVariables( installFileAsXml );
	
	return false;
}

QDomNode getChildNodeByName( QDomNode parentNode, QString nodeName )
{
	if( parentNode.isNull() )
		return QDomNode();

	if( !parentNode.hasChildNodes() )
		return QDomNode();
		
	QDomNode childNode = parentNode.firstChild();
	while( !childNode.isNull() )
	{
		if( childNode.toElement().tagName() == nodeName )
			break;

		childNode = childNode.nextSibling();
	}
	
	return childNode;
}

void CoreInstaller::readVariables( QDomDocument& installFileAsXml )
{
	// get root
	QDomNode root = installFileAsXml.documentElement();
	
	// get the "variables" node
	QDomNode variablesNode = getChildNodeByName( root, "variables" );
	if( variablesNode.isNull() )
	{
		qDebug( "variablesNode is null" );
		return;
	}
	
	// reading the vars defiend by the user
	qDebug( "reading variables" );
	QDomNode variableNode = variablesNode.firstChild();
	while( !variableNode.isNull() )
	{
		_variables.insert( variableNode.toElement().tagName(), variableNode.attributes().namedItem("value").toElement().tagName() );
	
		variableNode = variableNode.nextSibling();
	}
	
	// now add the system provided variables
	int index = 0;
	while( *(environ+index) )
	{
		QString expression = *(environ+index);
		QStringList variableVsValue = expression.split( "=" );
		if( 0 == variableVsValue.size() )
			continue;

		QString variableName = variableVsValue.size() ? variableVsValue.at(0) : "";
		QString variablevalue = (variableVsValue.size()>1) ? variableVsValue.at(1) : "";

		_variables.insert( variableName, variablevalue );
	}
	
	// ok, now we're done with the reading, now we must replace the content of the variables that have
	// other variables inside
	QMap<QString,QString>::iterator first = _variables.begin();
	QMap<QString,QString>::iterator last = _variables.end();
	QMap<QString,QString>::iterator it;
	for( it = first; it != last ; it++ )
	{
		QString variableName = it.key();
		QString value = it.value();
		
		while( value.contains("@") )
		{
			int startPos = value.indexOf( "@" );
			int shift = 1;
			while( isalpha(value.at(startPos+shift).toAscii()) )
				shift++;
			
			QString variableToLookFor = value.mid( startPos, shift );
			
			if( !_variables.contains(variableToLookFor) )
				continue;
			
			value.remove( startPos, shift );
			
			value.insert( startPos, _variables[variableToLookFor] );
		}
	}
	
	// if we are in debug mode, then dump some data
	if( !_weAreInDebugMode )
		return;

	qDebug("System environement variables detected:");
	QMapIterator<QString,QString> mapIterator( _variables );
	while( mapIterator.hasNext() )
	{
		qDebug( QString("%1 = %2").arg(mapIterator.peekNext().key()).arg(mapIterator.peekNext().value()).toAscii().data() );
		
		mapIterator.next();
	}
}
