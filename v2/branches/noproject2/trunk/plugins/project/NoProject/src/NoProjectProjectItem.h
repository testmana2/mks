#ifndef NOPROJECTPROJECTITEM_H
#define NOPROJECTPROJECTITEM_H

#include "ProjectItem.h"
#include "UINoProjectProjectSettings.h"

class ProjectPlugin;

class NoProjectProjectItem : public ProjectItem
{
	friend class UINoProjectProjectSettings;
	
public:
	NoProjectProjectItem ();

	void close ();
	
public slots:
	void editSettings();

};

#endif // NOPROJECTPROJECTITEM_H
