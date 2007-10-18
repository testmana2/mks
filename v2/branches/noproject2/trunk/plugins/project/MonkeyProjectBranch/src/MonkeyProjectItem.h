/****************************************************************************
**
**      Created using kate
** Author    : Kopats Andrei aka hlamer <hlamer@tut.by>
** Project   : MonkeyProject project type 
** FileName  : MonkeyProjectItem.h
** Date      : 2007-09-28
** License   : GPL
** Comment   : MonkeyProject project  class
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef NOPROJECTPROJECTITEM_H
#define NOPROJECTPROJECTITEM_H

#include "ProjectItem.h"

//class ProjectPlugin;
class UIMonkeyProjectSettings;
class QAction;

class MonkeyProjectItem : public ProjectItem
{
struct Target
{
	QString text;
	QString command;
	QAction* action;  //may be NULL or valid pointer
};
	friend class UIMonkeyProjectSettings;
	
public:
	MonkeyProjectItem ();
	~MonkeyProjectItem ();
	
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
