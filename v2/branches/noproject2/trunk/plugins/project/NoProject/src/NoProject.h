#ifndef NOPROJECT_H
#define NOPROJECT_H

#include "ProjectPlugin.h"
class NoProjectProjectItem;

class NoProject : public ProjectPlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin ProjectPlugin )

public:
	NoProject();
	ProjectItem* generateProjectItem ();
	
	bool setEnabled( bool b);
};

#endif // NOPROJECT_H
