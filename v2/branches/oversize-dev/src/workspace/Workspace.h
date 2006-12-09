#ifndef WORKSPACE_H
#define WORKSPACE_H
//
#include "pTabWorkspace.h"
//
#include <QPointer>
//
class ProjectManager;
//
class Workspace : public pTabWorkspace
{
	Q_OBJECT
	//
public:
	static Workspace* self( QWidget* = 0 );
	virtual ~Workspace();
	//
	void initialize();
	//
	ProjectManager* projectManager();
	//
private:
	Workspace( QWidget* = 0 );
	//
	static QPointer<Workspace> mSelf;
	//
};
//
#endif // WORKSPACE_H
