
// Qt's headers
#include <QtCore/QCoreApplication>

// local headers
#include <SimpleSettings>

int main(int argc, char* argv[])
{
	QCoreApplication coreApplication( argc, argv );

	SimpleSettings simpleSettings;
	simpleSettings.setSettingsFile( "settings.xml" );

	// adding two keys at root level
	simpleSettings.addKey( "a" );
	simpleSettings.setKey( "a", "value-a" );

	simpleSettings.addKey( "b" );
	simpleSettings.setKey( "b", "value-b" );

	// adding two keys at level one deep
	simpleSettings.addKey( "a-in-deep", "levelOne" );
	simpleSettings.setKey( "a-in-deep", "value-a-in-deep", "levelOne" );

	simpleSettings.addKey( "b-in-deep", "levelOne" );
	simpleSettings.setKey( "b-in-deep", "value-b-in-deep", "levelOne" );
	
	// adding two keys at level two deep
	simpleSettings.addKey( "a-in-deep", "levelOne/levelTwo" );
	simpleSettings.setKey( "a-in-deep", "value-a-in-deep", "levelOne/levelTwo" );

	simpleSettings.addKey( "b-in-deep", "levelOne/levelTwo" );
	simpleSettings.setKey( "b-in-deep", "value-b-in-deep", "levelOne/levelTwo" );
	
	// adding two keys at level three deep
	simpleSettings.addKey( "a-in-deep", "levelOne/levelTwo/levelThree" );
	simpleSettings.setKey( "a-in-deep", "value-a-in-deep", "levelOne/levelTwo/levelThree" );

	simpleSettings.addKey( "b-in-deep", "levelOne/levelTwo/levelThree" );
	simpleSettings.setKey( "b-in-deep", "value-b-in-deep", "levelOne/levelTwo/levelThree" );
	
	// adding one key at level one with different name for path(tag)
	simpleSettings.addKey( "a-in-deep", "levelOneSecondEdition" );
	simpleSettings.setKey( "a-in-deep", "value-a-in-deep", "levelOneSecondEdition" );
	
	// adding one key at root level
	simpleSettings.addKey( "c" );
	simpleSettings.setKey( "c", "value-c" );

	simpleSettings.saveSettings();

	return 0;
}
