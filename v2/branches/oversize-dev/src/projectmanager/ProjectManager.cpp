#include "ProjectManager.h"
#include "MenuBar.h"
//
QPointer<ProjectManager> ProjectManager::mSelf = 0L;
//
ProjectManager* ProjectManager::self( QObject* p )
{
	if ( !mSelf )
		mSelf = new ProjectManager( p );
	return mSelf;
}
//
ProjectManager::ProjectManager( QObject* p )
	: QObject( p )
{
}
//
ProjectManager::~ProjectManager()
{
}
//
void ProjectManager::initialize()
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
AbstractProject* ProjectManager::project()
{
}
//
QList<AbstractProject*> ProjectManager::projects()
{
}
//
AbstractFilesTree* ProjectManager::filesTree()
{
}
//
AbstractClassViewer* ProjectManager::classViewer()
{
}
//
AbstractCompiler* ProjectManager::compiler()
{
}
//
AbstractDebugger* ProjectManager::debugger()
{
}
