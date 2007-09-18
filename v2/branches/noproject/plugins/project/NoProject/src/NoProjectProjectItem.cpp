#include "NoProjectProjectItem.h"
#include <QDebug>
#include "ProjectPlugin.h"

NoProjectProjectItem::NoProjectProjectItem (ProjectPlugin* parent)
	: ProjectItem()
{
 	parentPlugin = parent;
	setType( ProjectsModel::ProjectType );
	setValue( "Project" );
	setFilePath ("toto.noproject");
}

void NoProjectProjectItem::editSettings()
{
	new UINoProjectProjectSettings (this);
}