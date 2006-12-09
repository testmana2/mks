#include "ProjectsManager.h"
#include "MenuBar.h"
//
QPointer<ProjectsManager> ProjectsManager::mSelf = 0L;
//
ProjectsManager* ProjectsManager::self( QObject* p )
{
	if ( !mSelf )
		mSelf = new ProjectsManager( p );
	return mSelf;
}
//
ProjectsManager::ProjectsManager( QObject* p )
	: QObject( p )
{
}
//
ProjectsManager::~ProjectsManager()
{
}
//
void ProjectsManager::initialize()
{
	MenuBar* mb = MenuBar::self();
	// project menu
	mb->beginGroup( "mProject" );
		mb->menu( "mSave" )->setEnabled( false );
		mb->menu( "mClose" )->setEnabled( false );
		mb->menu( "mShow" )->setEnabled( false );
		mb->menu( "mAdd" )->setEnabled( false );
		mb->action( "aSettings" )->setEnabled( false );
	mb->endGroup();
	// compiler menu
	mb->beginGroup( "mBuild" );
		mb->menu( "mBuild" )->setEnabled( false );
		mb->menu( "mRebuild" )->setEnabled( false );
		mb->menu( "mClean" )->setEnabled( false );
		mb->menu( "mDistClean" )->setEnabled( false );
		mb->action( "aExecute" )->setEnabled( false );
		mb->action( "aStop" )->setEnabled( false );
		mb->action( "alupdate" )->setEnabled( false );
		mb->action( "alrelease" )->setEnabled( false );
		mb->action( "aDistCleanBuildExecute" )->setEnabled( false );
	mb->endGroup();
}
//
AbstractProject* ProjectsManager::project()
{
}
//
QList<AbstractProject*> ProjectsManager::projects()
{
}
//
AbstractFilesTree* ProjectsManager::filesTree()
{
}
//
AbstractClassViewer* ProjectsManager::classViewer()
{
}
//
AbstractCompiler* ProjectsManager::compiler()
{
}
//
AbstractDebugger* ProjectsManager::debugger()
{
}
