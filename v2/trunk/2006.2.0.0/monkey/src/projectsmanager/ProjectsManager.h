#ifndef PROJECTSMANAGER_H
#define PROJECTSMANAGER_H
//
#include <QPointer>
//
#include "MonkeyExport.h"
//
class AbstractProject;
class AbstractFilesTree;
class AbstractClassViewer;
class AbstractCompiler;
class AbstractDebugger;
//
class Q_MONKEY_EXPORT ProjectsManager : public QObject
{
	Q_OBJECT
	//
public:
	static ProjectsManager* self( QObject* = 0 );
	~ProjectsManager();
	//
	void initialize();
	//
	AbstractProject* project();
	QList<AbstractProject*> projects();
	AbstractFilesTree* filesTree();
	AbstractClassViewer* classViewer();
	AbstractCompiler* compiler();
	AbstractDebugger* debugger();
	//
private:
	ProjectsManager( QObject* = 0 );
	//
	static QPointer<ProjectsManager> mSelf;
	//
};
//
#endif // PROJECTSMANAGER_H
