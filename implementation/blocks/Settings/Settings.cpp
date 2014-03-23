
// Qt headers
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QCoreApplication>

#include <QtXml/QDomText>
#include <QtXml/QDomNode>
#include <QtXml/QDomNodeList>


// local headers
#include "Settings.h"

Settings::Settings() :
    _settingsFilePath( "" )
{}

Settings::~Settings()
{}

QString Settings::getSettingsFileForApp()
{
    return QCoreApplication::applicationFilePath() + ".settings";
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- Utility functions
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
QDomNode getChildByName( QDomNode parent, QString nodeName )
{
    QDomNodeList nodes = parent.childNodes();
    for( int index = 0; index < nodes.count(); ++index )
    {
        QDomNode node = nodes.at( index );
        if( nodeName == node.toElement().tagName() )
            return node;
    }

    return QDomNode();
}

QDomNode getNodeFromKeyName( QDomDocument domDocument, QString keyName )
{
    // pointing the parent node to be the root of the DOM document
    QDomNode node = domDocument.documentElement();

    QStringList listOfNodeNames = keyName.split( "/" );
    foreach( QString nodeName, listOfNodeNames )
    {
        node = getChildByName( node, nodeName );
        if( node.isNull() )
            return QDomNode();
    }

    return node;
}




// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- Methods Implementation
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
bool Settings::setFile( QString settingsFilePath, bool createIfNotExist )
{
    _settingsFilePath = settingsFilePath;

    QFile file( _settingsFilePath );

    // check if the file exists
    if( false == QFile::exists(_settingsFilePath) )
    {
        // creating settings file if does not exist
        if( true == createIfNotExist )
        {
            if( false == file.open(QIODevice::WriteOnly) )
                return false;
        }

        // no file was found and it was not specified to create one
        else
            return false;

        // file is there or it was created Ok
        const QString c_data = "<?xml version=\"1.0\" ?><settings></settins>";

        file.write( c_data.toAscii() );
        file.close();
    }

    // checking if we can open file for reading
    if( false == file.open(QIODevice::ReadOnly) )
        return false;

    // loading the settings file
    _domDocument.setContent( &file );
    file.close();

    // returning success
    return true;
}

bool Settings::save()
{
    QFile file( _settingsFilePath );
    if( false == file.open(QIODevice::WriteOnly) )
        return false;

    // saving content
    QTextStream textStream( &file );
    _domDocument.save( textStream, 4 );

    return true;
}


void Settings::addKey( QString keyName )
{
    // pointing the parent node to be the root of the DOM document
    QDomNode parent = _domDocument.documentElement();

    // seeking the target node and ensure it exists
    QStringList listOfNodeNames = keyName.split( "/" );
    foreach( QString nodeName, listOfNodeNames )
    {
        QDomNode child = getChildByName( parent, nodeName );
        if( child.isNull() )
        {
            QDomElement element = _domDocument.createElement( nodeName );
            child = parent.appendChild( element );
        }

        parent = child;
    }
}

void Settings::removeKey( QString keyName )
{
    QDomNode node = getNodeFromKeyName( _domDocument, keyName );

    if( node.isNull() )
        return;

    QDomNode parent = node.parentNode();

    parent.removeChild( node );
}

void Settings::setValueForKey( QString keyName, QString keyValue, bool addKeyIfNotPresent )
{
    QDomNode node = getNodeFromKeyName( _domDocument, keyName );
    if( node.isNull() )
    {
        if( addKeyIfNotPresent )
        {
            addKey( keyName );
            node = getNodeFromKeyName( _domDocument, keyName );
        }
        else
            return;
    }

    if( true == node.hasChildNodes() )
        node.removeChild( node.firstChild() );

    QDomText textValue = _domDocument.createTextNode( keyValue );
    node.appendChild( textValue );
}

void Settings::getValueForKey( QString keyName, QString& keyValue )
{
    QDomNode node = getNodeFromKeyName( _domDocument, keyName );

    if( node.isNull() )
        return;

    keyValue = node.toElement().text();
}

void Settings::setValuesForKey( QString keyName, QStringList& valuesList, bool addKeyIfNotPresent )
{
    QDomNode node = getNodeFromKeyName( _domDocument, keyName );
    if( node.isNull() )
    {
        if( addKeyIfNotPresent )
        {
            addKey( keyName );
            node = getNodeFromKeyName( _domDocument, keyName );
        }
        else
            return;
    }

    // removing all previous child nodes
    QDomNode childNode = node.firstChild();
    while( !childNode.isNull() )
    {
        node.removeChild( childNode );
        childNode = node.firstChild();
    }

    // setting the node's values
    foreach( QString value, valuesList )
    {
        QDomElement newElement = _domDocument.createElement( "value" );
        QDomNode valueNode = node.appendChild( newElement );

        // creating a new child node
        QDomText textValue = _domDocument.createTextNode( value );
        valueNode.appendChild( textValue );
    }
}

void Settings::getValuesForKey( QString keyName, QStringList& valueList )
{
    QDomNode node = getNodeFromKeyName( _domDocument, keyName );
    if( node.isNull() )
        return;

    QDomNode value = node.firstChild();
    QDomNodeList childNodes = node.childNodes();
    for( int index = 0; index < childNodes.count(); ++index )
        valueList.append( childNodes.at(index).toElement().text() );
}

void Settings::getChildrenNamesForKey( QString keyName, QStringList& valueList )
{
    QDomNode node = getNodeFromKeyName( _domDocument, keyName );
    if( node.isNull() )
        node = _domDocument.documentElement();

    QDomNodeList nodes = node.childNodes();
    for( int index = 0; index < nodes.count(); ++index )
    {
        QDomNode node = nodes.at( index );
        valueList.append( node.toElement().tagName() );
    }
}
