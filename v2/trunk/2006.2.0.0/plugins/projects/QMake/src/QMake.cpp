#include "QMake.h"
#include "QMakeProjectItemModel.h"
#include "QMakeProjectProxy.h"
#include "ProjectsManager.h"
//
QMake::~QMake()
{
	if ( isInstalled() )
		uninstall();
}
//
void QMake::initialize( Workspace* w )
{
	ProjectPlugin::initialize( w );
	// plugin infos
	mPluginInfos.Caption = tr( "Qt QMake Project Plugin Manager" );
	mPluginInfos.Description = tr( "This plugin allow you to use Qt QMake project files" );
	mPluginInfos.Type = PluginInfos::iProject;
	mPluginInfos.Name = "QMake";
	mPluginInfos.Version = "1.0.0";
	mPluginInfos.Installed = false;
	//
	mExtensions = QStringList() << "pro";
}
//
bool QMake::install()
{
	mPluginInfos.Installed = true;
	return true;
}
//
bool QMake::uninstall()
{
	mPluginInfos.Installed = false;
	return true;
}
//
QStringList QMake::filters() const
{
	QStringList e = extensions();
	QStringList f;
	foreach ( QString s, e )
		f << QString( "*.%1" ).arg( s );
	return QStringList() << QString( "QMake Project Files ( %1 )" ).arg( f.join( " " ) );
}
//
bool QMake::openProject( const QString& s, AbstractProjectProxy* py )
{
	// s is normally clean and canonical
	// check if project is already open
	foreach ( AbstractProjectProxy* p, AbstractProjectProxy::all() )
	{
		if ( p->project()->filePath() == s )
		{
			mWorkspace->projectsManager()->setCurrentProxy( p );
			return false;
		}
	}
	// create a new qmake project
	AbstractProjectProxy* mProxy = new QMakeProjectProxy( new QMakeProjectItemModel( s ) );
	// open project
	if ( mProxy->project()->open() )
	{
		// add it to projects manager
		mWorkspace->projectsManager()->addProxy( mProxy, py );
		// open subprojects
		foreach ( QString h, mProxy->project()->subProjects() )
			openProject( h, mProxy );
		// return true
		return true;
	}
	else
		delete mProxy->sourceModel(); // it will delete proxy too
	return false;
}
//
Q_EXPORT_PLUGIN2( ProjectQMake, QMake )
