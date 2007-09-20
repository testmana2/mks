#ifndef NOPROJECT_H
#define NOPROJECT_H

#include "ProjectPlugin.h"

class NoProject : public ProjectPlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin ProjectPlugin )

public:
	NoProject();
	ProjectItem* generateProjectItem ();
};

#endif // NOPROJECT_H
