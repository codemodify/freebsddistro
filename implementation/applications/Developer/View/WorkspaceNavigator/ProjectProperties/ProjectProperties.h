#ifndef __ProjectProperties_H__
#define __ProjectProperties_H__

// qt headers
#include <QtCore/QString>
#include <QtGui/QDialog>

// local headers
#include "ui_ProjectProperties.h"
#include "../WorkspaceFile.h"

class ProjectProperties : public QDialog, public Ui::ProjectProperties
{
	Q_OBJECT

    public:
	    ProjectProperties( WorkspaceFile& workspaceFile, QString currentProject );


    public slots:
        void slot_BrowseFile();
        void slot_AddBuildTarget();
        void slot_RemoveBuildTarget();
        
        void runActionTextEdited                ( const QString& text );
        void debugActionTextEdited              ( const QString& text );
        void buildTargetActionEdited            ( const QString& text );
        void buildTargetsCurrentItemTextChanged ( QListWidgetItem* item );
        
        void buildTargetsCurrentItemChanged( QListWidgetItem* current, QListWidgetItem* previous );
        
    protected:
        void closeEvent( QCloseEvent* event );
        

    private:
        WorkspaceFile&  _workspaceFile;
        QString         _currentProject;

};

#endif

