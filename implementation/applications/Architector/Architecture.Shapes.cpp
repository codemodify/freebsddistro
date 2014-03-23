
// blocks
#include <Settings/Settings.h>

// local headers
#include "Architecture.Shapes.h"


Shapes::Shapes() :
    QObject()
{}

Shapes* Shapes::loadFromFile( QString filePath )
{
    Shapes* shapes = new Shapes();

    Settings    settings;
                settings.setFile( filePath );

    // read the Global area
    QString renderer;
    settings.getValueForKey( "Global/Renderer", renderer );

    // read the Problem area
    // FIXME: ...    

    // read the Analyze area
    // FIXME: ...    

    // read the Implementation area
    QStringList classes;
    settings.getValuesForKey( "Implementation/Classes", classes );

    return shapes;
}

void Shapes::saveToFile( QString filePath )
{
    Settings    settings;
                settings.setFile( filePath );

    // save the Global area
    settings.setValueForKey( "Global/Renderer", _renderer );

    // save the Problem area
    // FIXME: ...    

    // save the Analyze area
    // FIXME: ...    

    // save the Implementation area
    settings.setValuesForKey( "Implementation/Classes", _classes );

    settings.save();
}

QString Shapes::getRenderer()
{
    return _renderer;
}
