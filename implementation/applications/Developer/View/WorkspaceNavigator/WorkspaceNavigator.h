#ifndef __WorkspaceNavigator_H__
#define __WorkspaceNavigator_H__

// Qt's headers
#include <QtCore/QProcess>

// local headers
#include "View/FileSystemBrowser/FileSystemBrowser.h"

// forwards
class WorkspaceFile;


class WorkspaceNavigator : public FileSystemBrowser
{
	Q_OBJECT

	public:
		WorkspaceNavigator( QString workspacePath );
		~WorkspaceNavigator();

	signals:
		void search( QString folder );
		void makeOutputConsoleVisible();

    public:
        static bool isWorkspaceValid( QString workspaceFolder );
        static void createWorkspace( QString workspaceFolder );


    // inherited stuff
	protected:
		void initMenu();
		void contextMenuAboutToPopup();
		void initTitleBar();
		void initContent();


    // project management
	private slots:
        void workspaceNavigatorSetAsCurrentProject();
		void workspaceNavigatorCreateProject();
		void workspaceNavigatorDeleteProject();

		void workspaceNavigatorRunProject();
		void workspaceNavigatorDebugProject();
		void workspaceNavigatorExecuteBuildTarget();
		void workspaceNavigatorEditBuildTargets();

		void workspaceNavigatorExecuteShortcutCompile();
		void workspaceNavigatorExecuteShortcutSearchInFiles();

		void runBuildTarget( QString action );


    // helpers
    private:
        QString getCurrentProject();

	signals:
	    void outputConsoleAdd       ( QString output );
	    void outputConsoleAddWarning( QString output );
	    void outputConsoleAddError  ( QString output );
	    void outputConsoleClear     ();

    private slots:
	    void readBuildTargetStandardError();
	    void readBuildTargetStandardOutput();
	    void readBuildFinishStatus( int exitCode, QProcess::ExitStatus exitStatus );

	private:
        QString         _currentProject;
		QMenu           _buildMenu;
		WorkspaceFile*  _workspaceFile;
		QProcess        _process;
};

#endif

