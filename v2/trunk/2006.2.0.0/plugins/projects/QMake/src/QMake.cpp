#include "QMake.h"
#include "QMakeProjectItemModel.h"
#include "QMakeProjectProxy.h"
#include "ProjectsManager.h"
#include "UISettingsQMake.h"
//
#include <QApplication>
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
	mPluginInfos.Type = BasePlugin::iProject;
	mPluginInfos.Name = "QMake";
	mPluginInfos.Version = "0.5.0";
	mPluginInfos.Installed = false;
	//
	mExtensions = QStringList() << "pro";
}
//
QWidget* QMake::settingsWidget()
{
	return new UISettingsQMake;
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
	if ( !isInstalled() )
		return QStringList();
	QStringList e = extensions();
	QStringList f;
	foreach ( QString s, e )
		f << QString( "*.%1" ).arg( s );
	return QStringList() << QString( "QMake Project Files ( %1 )" ).arg( f.join( " " ) );
}
//
bool QMake::openProject( const QString& s, AbstractProjectProxy* py )
{
	if ( !isInstalled() )
		return false;
	// s is normally clean and canonical
	// check if project is already open
	foreach ( AbstractProjectProxy* p, AbstractProjectProxy::all() )
	{
		if ( p->project()->filePath() == s )
		{
			mWorkspace->projectsManager()->setCurrentProxy( p );
			return true;
		}
	}
	// create a new qmake project
	QMakeProjectItemModel* mProject = new QMakeProjectItemModel( s );
	// if we can open project
	if ( mProject->open() )
	{
		AbstractProjectProxy* mProxy = new QMakeProjectProxy( mProject );
		// add it to projects manager
		mWorkspace->projectsManager()->addProxy( mProxy, py );
		// open subprojects
		foreach ( QString h, mProxy->project()->subProjects() )
		{
			QApplication::processEvents( QEventLoop::ExcludeUserInputEvents );
			openProject( h, mProxy );
		}
		// set root project the current project
		mWorkspace->projectsManager()->setCurrentProxy( mProxy );
		// show/hide projects lsit according to projects list count
		bool b = mWorkspace->projectsManager()->rootProjects().count() > 1;
		if ( !b && mProject->subProjects().count() )
			b = true;
		mWorkspace->projectsManager()->setTreeProjectsVisible( b );
		// return true
		return true;
	}
	else
		delete mProject;
	return false;
}
//
Q_EXPORT_PLUGIN2( ProjectQMake, QMake )
