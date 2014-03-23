#ifndef __WorkspaceFile_H__
#define __WorkspaceFile_H__

// blocks
#include <Settings/Settings.h>


class WorkspaceFile
{
    public:
	    WorkspaceFile( QString workspaceFolder );
	    ~WorkspaceFile();
	    
	    static const QString FileName;

        void deleteProject( QString project );

    // run / debug
    public:
        QString getRunActionForProject          ( QString project                                       );
        QString getDebugActionForProject        ( QString project                                       );
        
        void    setRunActionForProject          ( QString project, QString runAction                    );
        void    setDebugActionForProject        ( QString project, QString debugAction                  );

    // build targets
    public:
        void    deleteBuildTarget               ( QString project, int buildTargetIndex                 );
        
        int     getBuildTargetsCountForProject  ( QString project                                       );
        QString getBuildTargetNameForProject    ( QString project, int buildTargetIndex                 );
        QString getBuildTargetActionForProject  ( QString project, int buildTargetIndex                 );
        
        void    setBuildTargetsCountForProject  ( QString project, int buildTargetsCount                );
        void    setBuildTargetNameForProject    ( QString project, int buildTargetIndex, QString name   );
        void    setBuildTargetActionForProject  ( QString project, int buildTargetIndex, QString action );


    private:
	    Settings _settings;

};

#endif
