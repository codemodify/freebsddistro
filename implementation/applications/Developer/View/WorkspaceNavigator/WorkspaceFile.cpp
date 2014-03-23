
// local headers
#include "WorkspaceFile.h"

// static members
const QString WorkspaceFile::FileName = ".workspace";

// structure of the .workspace file
#define RunAction( project )    ( QString("%1/runAction").arg(project) )
#define DebugAction( project )  ( QString("%1/debugAction").arg(project) )

#define BuildTargetsNames( project )    ( QString("%1/buildTargetsNames").arg(project) )
#define BuildTargetActions( project )   ( QString("%1/buildTargetsActions").arg(project) )


WorkspaceFile::WorkspaceFile( QString workspaceFolder )
{
    _settings.setFile( workspaceFolder + "/" + FileName );
}

WorkspaceFile::~WorkspaceFile()
{
    _settings.save();
}

void WorkspaceFile::deleteProject( QString project )
{
    _settings.removeKey( project );
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// run / debug
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
QString WorkspaceFile::getRunActionForProject( QString project )
{
    QString runAction;
    _settings.getValueForKey( RunAction(project), runAction );
    
    return runAction;
}

QString WorkspaceFile::getDebugActionForProject( QString project )
{
    QString debugAction;
    _settings.getValueForKey( DebugAction(project), debugAction );
    
    return debugAction;
}

void WorkspaceFile::setRunActionForProject( QString project, QString runAction )
{
    _settings.setValueForKey( RunAction(project), runAction );
}

void WorkspaceFile::setDebugActionForProject( QString project, QString debugAction )
{
    _settings.setValueForKey( DebugAction(project), debugAction );
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// build targets
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void WorkspaceFile::deleteBuildTarget( QString project, int buildTargetIndex )
{
    QStringList stringList;
    
    // build targets
    _settings.getValuesForKey( BuildTargetsNames(project), stringList );
    stringList.removeAt( buildTargetIndex );
    _settings.setValuesForKey( BuildTargetsNames(project), stringList );
    
    stringList.clear();
    
    _settings.getValuesForKey( BuildTargetActions(project), stringList );
    stringList.removeAt( buildTargetIndex );
    _settings.setValuesForKey( BuildTargetActions(project), stringList );
}

int WorkspaceFile::getBuildTargetsCountForProject( QString project )
{
    QStringList stringList;

    _settings.getValuesForKey( BuildTargetsNames(project), stringList );
    
    return stringList.count();
}

QString WorkspaceFile::getBuildTargetNameForProject( QString project, int buildTargetIndex )
{
    QStringList stringList;
    _settings.getValuesForKey( BuildTargetsNames(project), stringList );
    
    return stringList.value( buildTargetIndex );
}

QString WorkspaceFile::getBuildTargetActionForProject( QString project, int buildTargetIndex )
{
    QStringList stringList;
    _settings.getValuesForKey( BuildTargetActions(project), stringList );
    
    return stringList.value( buildTargetIndex );
}

void WorkspaceFile::setBuildTargetNameForProject( QString project, int buildTargetIndex, QString name )
{
    QStringList stringList;
    
    _settings.getValuesForKey( BuildTargetsNames(project), stringList );
    
    if( stringList.count() <= buildTargetIndex )
        stringList.append( name );
    else
        stringList[ buildTargetIndex ] = name;

    _settings.setValuesForKey( BuildTargetsNames(project), stringList );
 }

void WorkspaceFile::setBuildTargetActionForProject( QString project, int buildTargetIndex, QString action )
{
   QStringList stringList;

    _settings.getValuesForKey( BuildTargetActions(project), stringList );
    
    if( stringList.count() <= buildTargetIndex )
        stringList.append( action );
    else
        stringList[ buildTargetIndex ] = action;

    _settings.setValuesForKey( BuildTargetActions(project), stringList );    
}

