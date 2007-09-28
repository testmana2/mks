/****************************************************************************
**
**      Created using kate
** Author    : Kopats Andrei aka hlamer <hlamer@tut.by>
** Project   : NoProject plugin
** FileName  : NoProject.h
** Date      : 2007-09-28
** License   : GPL
** Comment   : NoProject plugin class
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

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
