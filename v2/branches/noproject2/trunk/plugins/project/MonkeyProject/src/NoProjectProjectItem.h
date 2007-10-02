/****************************************************************************
**
**      Created using kate
** Author    : Kopats Andrei aka hlamer <hlamer@tut.by>
** Project   : NoProject project type 
** FileName  : NoProjectProjectItem.cpp
** Date      : 2007-09-28
** License   : GPL
** Comment   : NoProject project  class
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef NOPROJECTPROJECTITEM_H
#define NOPROJECTPROJECTITEM_H

#include "ProjectItem.h"

//class ProjectPlugin;
class UINoProjectProjectSettings;
class QAction;

class NoProjectProjectItem : public ProjectItem
{
struct Target
{
	QString text;
	QString command;
	QAction* action;  //may be NULL or valid pointer
};
	friend class UINoProjectProjectSettings;
	
public:
	NoProjectProjectItem ();
	~NoProjectProjectItem ();
	
	void setValue (QString);	
	void close ();
	
public slots:
	void editSettings();
	void buildMenuTriggered ();
	void removeSelfFromMenu (QMenu* menu = NULL);
	void addSelfToMenu (QMenu* menu = NULL);
	ProjectPlugin* getParentPlugin ();
	void save( bool = true );
	bool openProject( const QString&);

private:
	QList<Target> targets;
	QString projectPath;
};

#endif // NOPROJECTPROJECTITEM_H
