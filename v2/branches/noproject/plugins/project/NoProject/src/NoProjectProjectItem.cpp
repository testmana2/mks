#include "NoProjectProjectItem.h"
#include <QDebug>
#include "ProjectPlugin.h"

NoProjectProjectItem::NoProjectProjectItem (ProjectPlugin* parent)
{
 	parentPlugin = parent;
	setFilePath ("");
	editSettings ();
}

void NoProjectProjectItem::editSettings()
{
	new UINoProjectProjectSettings (this);
}