#include "NoProject.h"
#include "NoProjectProjectItem.h"

NoProject::NoProject()
{
	// set plugin infos
	mPluginInfos.Caption = tr( "NoProject" );
	mPluginInfos.Description = tr( "Plugin for managing projects without any project file" );
	mPluginInfos.Author = "Kopats Andrei aka hlamer <hlamer@tut.by>";
	mPluginInfos.Type = BasePlugin::iProject;
	mPluginInfos.Name = "NoProject";
	mPluginInfos.Version = "0.0.1";
	mPluginInfos.Enabled = false;
	mSuffixes["No project"] = QStringList(".noproject");
}

ProjectItem* NoProject::generateProjectItem()
{
	// don t open project if plugin is not enabled
	if ( !isEnabled() )
		return NULL;
	// crete root project item and return it
	return dynamic_cast<ProjectItem*> (new NoProjectProjectItem );
}

bool NoProject::setEnabled (bool en)
{
	mPluginInfos.Enabled = en;
	return true;
};

Q_EXPORT_PLUGIN2( ProjectNoProject, NoProject )
