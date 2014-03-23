
// Qt headers
#include <QtCore/QFile>

// local headers
#include "Components/Components.h"

QDomNode IspComponents::Dom::getChildNodeByName( QDomNode parentNode, QString childTagNameToLookFor )
{
	if( parentNode.isNull() )
		return QDomNode();
		
	QDomNode node = parentNode.firstChild();
	while( true == !node.isNull() )
	{
		if( childTagNameToLookFor == node.nodeName() )
			break;

		node = node.nextSibling();
	}
	
	return node;
}

QDomNode IspComponents::Dom::loadXmlFileAndGetRootNode( QString settingsFilePath, QDomDocument& settingsFileAsXml )
{
	QFile settingsFile( settingsFilePath );

	if( false == settingsFile.open(QFile::ReadOnly) )
		return QDomNode();

	if( false == settingsFileAsXml.setContent(&settingsFile) )
	{
		settingsFile.close();
		return QDomNode();
	}
	settingsFile.close();

	return settingsFileAsXml.documentElement();
}
