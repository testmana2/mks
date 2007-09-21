#ifndef NOPROJECTPROJECTITEM_H
#define NOPROJECTPROJECTITEM_H

#include "ProjectItem.h"

class ProjectPlugin;
class UINoProjectProjectSettings;
class QAction;

class NoProjectProjectItem : public ProjectItem
{
struct target
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
    void removeSelfFromMenu (QMenu* menu);
    void addSelfToMenu (QMenu* menu);

private:
	QList<target> targets;
	void makeTarget (int targetIndex);


};

#endif // NOPROJECTPROJECTITEM_H
