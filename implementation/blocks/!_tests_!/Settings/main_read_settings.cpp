
// Qt's headers
#include <QtCore/QCoreApplication>
#include <QtCore/QString>

// local headers
#include <SimpleSettings>
#include <Logger>

int main(int argc, char* argv[])
{
	QCoreApplication coreApplication( argc, argv );

	SimpleSettings simpleSettings;
	simpleSettings.setSettingsFile( "settings.xml" );
	
	QString value;

	// getting two keys at root level
	value = simpleSettings.getKey( "a" ); Logger::getInstance()->addDebug( value );
	value = simpleSettings.getKey( "b" ); Logger::getInstance()->addDebug( value );

	// getting two keys at level one deep
	value = simpleSettings.getKey( "a-in-deep", "levelOne" ); Logger::getInstance()->addDebug( value );
	value = simpleSettings.getKey( "b-in-deep", "levelOne" ); Logger::getInstance()->addDebug( value );
	
	// getting two keys at level two deep
	value = simpleSettings.getKey( "a-in-deep", "levelOne/levelTwo" ); Logger::getInstance()->addDebug( value );
	value = simpleSettings.getKey( "b-in-deep", "levelOne/levelTwo" ); Logger::getInstance()->addDebug( value );
	
	// getting two keys at level three deep
	value = simpleSettings.getKey( "a-in-deep", "levelOne/levelTwo/levelThree" ); Logger::getInstance()->addDebug( value );
	value = simpleSettings.getKey( "b-in-deep", "levelOne/levelTwo/levelThree" ); Logger::getInstance()->addDebug( value );
	
	// getting one key at level one with different name for path(tag)
	value = simpleSettings.getKey( "a-in-deep", "levelOneSecondEdition" ); Logger::getInstance()->addDebug( value );

	// getting one key at root level
	value = simpleSettings.getKey( "c" ); Logger::getInstance()->addDebug( value );

	return 0;
}
