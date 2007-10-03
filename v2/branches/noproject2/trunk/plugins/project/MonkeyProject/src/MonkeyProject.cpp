/****************************************************************************
**
**      Created using kate
** Author    : Kopats Andrei aka hlamer <hlamer@tut.by>
** Project   : MonkeyProject plugin
** FileName  : MonkeyProject.cpp
** Date      : 2007-09-28
** License   : GPL
** Comment   : MonkeyProject plugin class
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "MonkeyProject.h"
#include "MonkeyProjectItem.h"

MonkeyProject::MonkeyProject()
{
	// set plugin infos
	mPluginInfos.Caption = tr( "MonkeyProject" );
	mPluginInfos.Description = tr( "Plugin for managing projects without any project file" );
	mPluginInfos.Author = "Kopats Andrei aka hlamer <hlamer@tut.by>";
	mPluginInfos.Type = BasePlugin::iProject;
	mPluginInfos.Name = "MonkeyProject";
	mPluginInfos.Version = "0.0.1";
	mPluginInfos.Enabled = false;
	mSuffixes["No project"] = QStringList("*.noproject");
}

ProjectItem* MonkeyProject::generateProjectItem()
{
	// don t open project if plugin is not enabled
	if ( !isEnabled() )
		return NULL;
	// crete root project item and return it
	return dynamic_cast<ProjectItem*> (new MonkeyProjectItem );
}

bool MonkeyProject::setEnabled (bool en)
{
	mPluginInfos.Enabled = en;
	return true;
};

Q_EXPORT_PLUGIN2( ProjectMonkeyProject, MonkeyProject )
