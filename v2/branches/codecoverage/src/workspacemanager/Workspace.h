#ifndef WORKSPACE_H
#define WORKSPACE_H
//
#include "pTabWorkspace.h"
#include "MonkeyExport.h"
//
#include <QPointer>
//
class Settings;
class MenuBar;
class RecentsManager;
class TabToolBar;
class ProjectsManager;
class StatusBar;
class AbstractProjectProxy;
class AbstractChild;
class PluginsManager;
//
class Q_MONKEY_EXPORT Workspace : public pTabWorkspace
{
	Q_OBJECT
	//
public:
	static Workspace* self( QWidget* = 0 );
	virtual ~Workspace();
	//
	Settings* settings();
	MenuBar* menuBar();
	RecentsManager* recentsManager();
	TabToolBar* tabToolBar();
	ProjectsManager* projectsManager();
	PluginsManager* pluginsManager();
	StatusBar* statusBar();
	//
private:
	Workspace( QWidget* = 0 );
	void initialize();
	void updateTabNumbers( int );
	//
	static QPointer<Workspace> mSelf;
	//
public slots:
	void updateWorkspace();
	// tab signals
	void tabInserted( int );
	void tabRemoved( int );
	void tabChanged( int );
	//
	void openFile( const QString&, AbstractProjectProxy* = 0 );
	void openProject( const QString& );
	int addChild( AbstractChild*, const QString& );
	void childCloseEvent( AbstractChild*, QCloseEvent* );
	// file menu
	void fileOpen_triggered();
	void fileSaveCurrent_triggered();
	void fileSaveAll_triggered();
	void fileCloseCurrent_triggered();
	void fileCloseAll_triggered();
	void fileSaveAsTemplate_triggered();
	void fileQuickPrint_triggered();
	void filePrint_triggered();
	void fileExit_triggered();
	// edit menu
	void editSettings_triggered();
	void editUndo_triggered();
	void editRedo_triggered();
	void editCut_triggered();
	void editCopy_triggered();
	void editPaste_triggered();
	void editSearchReplace_triggered();
	void editGoTo_triggered();
	// view menu
	void viewNext_triggered();
	void viewPrevious_triggered();
	// project menu
	void projectNew_triggered();
	void projectOpen_triggered();
	void projectSaveCurrent_triggered();
	void projectSaveAll_triggered();
	void projectCloseCurrent_triggered();
	void projectCloseAll_triggered();
	void projectShowSource_triggered();
	void projectShowToDo_triggered();
	void projectShowChanges_triggered();
	void projectSettings_triggered();
	//
};
//
#endif // WORKSPACE_H
