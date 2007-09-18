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
	NoProjectProjectItem* it = new NoProjectProjectItem( parent );
	it->setType( ProjectsModel::EmptyType );
	it->setValue( "empty item" );
	append( it );
}

void NoProjectProjectItem::editSettings()
{
	new UINoProjectProjectSettings (this);
}