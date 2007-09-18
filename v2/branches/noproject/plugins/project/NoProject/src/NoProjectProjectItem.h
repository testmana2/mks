#ifndef NOPROJECTPROJECTITEM_H
#define NOPROJECTPROJECTITEM_H

#include "ProjectItem.h"
#include "UINoProjectProjectSettings.h"

class ProjectPlugin;

class NoProjectProjectItem : public ProjectItem
{
	friend class UINoProjectProjectSettings;
	
public:
	NoProjectProjectItem (ProjectPlugin* parent);

	QString buildCommand;
	QString cleanCommand;
	QString distCleanCommand;
	QString executeCommand;
 
public slots:
	void editSettings();

};

#endif // NOPROJECTPROJECTITEM_H
