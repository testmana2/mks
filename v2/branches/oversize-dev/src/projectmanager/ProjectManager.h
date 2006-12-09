#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H
//
#include <QObject>
#include <QPointer>
//
class AbstractProject;
class AbstractFilesTree;
class AbstractClassViewer;
class AbstractCompiler;
class AbstractDebugger;
//
class ProjectManager : public QObject
{
	Q_OBJECT
	//
public:
	static ProjectManager* self( QObject* = 0 );
	~ProjectManager();
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
	ProjectManager( QObject* = 0 );
	//
	static QPointer<ProjectManager> mSelf;
	//
};
//
#endif // PROJECTMANAGER_H
