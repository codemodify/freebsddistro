
// Qt hreaders
#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QCoreApplication>

// blocks
#include <FileSystemOperations/FileSystemOperations.h>

// local headers
#include "Deployer.h"
#include "Helper.h"


Deployer::Deployer( QString configurationFileName )
{
	if( configurationFileName.isEmpty() )
		configurationFileName = QCoreApplication::applicationFilePath() + ".deployer";

	QFile configurationFile( configurationFileName );
	if( true == configurationFile.open(QIODevice::ReadOnly) )
	{
		_domDocument.setContent( &configurationFile );
		configurationFile.close();
	}	
}

void Deployer::setSourceAndTarget( QString source, QString target )
{
	_source = source;
	_target = target;
}

void Deployer::deploy( QString scenarioName )
{
	QDomNode deployer = _domDocument.documentElement();
	if( deployer.isNull() || !deployer.hasChildNodes() )
		return;

	// seek to the right scenario
	QDomNode scenario = deployer.firstChild();
	while( !scenario.isNull() )
	{
		if(
             (QDomNode::CommentNode != scenario.nodeType()) &&
		    scenarioName == scenario.attributes().namedItem("name").nodeValue()
           )
			break;

		scenario = scenario.nextSibling();
	}

    FileSystemOperations*   fileSystemOperations = new FileSystemOperations();
                            fileSystemOperations->setDestroyOnFinish( true );

	// run the actions indicated in the scenario
	QDomNode action = scenario.firstChild();
	while( !action.isNull() )
	{
		ActionType actionType = getActionTypeByName( action.nodeName() );

		QString source = action.attributes().namedItem( "source" ).nodeValue();
		QString target = action.attributes().namedItem( "target" ).nodeValue();

		source.replace( "$source", _source );
		target.replace( "$target", _target );

		fileSystemOperations->createFolder( target );

		switch( actionType )
		{
			case eCopyContent:
				fileSystemOperations->copyFolderContent( source, target );
				break;

			default:
				break;
		}

		action = action.nextSibling();
	}
}

