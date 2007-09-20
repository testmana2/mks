#include "NoProjectProjectItem.h"
#include <QDebug>
#include "ProjectPlugin.h"

NoProjectProjectItem::NoProjectProjectItem ()
	: ProjectItem()
{
	setType( ProjectsModel::ProjectType );
	setValue( "Project" );
	setFilePath ("toto.noproject");
	editSettings();
}

void NoProjectProjectItem::editSettings()
{
	new UINoProjectProjectSettings (this);
}

void NoProjectProjectItem::close()
{
	delete (this);
}
