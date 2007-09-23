#ifndef NOPROJECTPROJECTITEM_H
#define NOPROJECTPROJECTITEM_H

#include "ProjectItem.h"

class ProjectPlugin;
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
	
	void close ();
	
public slots:
	void editSettings();
	void buildMenuTriggered ();
    void removeSelfFromMenu (QMenu* menu = NULL);
    void addSelfToMenu (QMenu* menu = NULL);
	ProjectPlugin* getParentPlugin ();
private:
	QList<Target> targets;

};

#endif // NOPROJECTPROJECTITEM_H
