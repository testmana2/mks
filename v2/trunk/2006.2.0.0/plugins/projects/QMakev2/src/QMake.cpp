#include "QMake.h"
#include "QMakeProjectModel.h"
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
	mExtensions = QStringList() << "pro" << "pri" << "inc";
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
bool QMake::openProject( const QString& s )
{
	if ( !isInstalled() )
		return false;
	// s is normally clean and canonical
	// check if project is already open
	foreach ( AbstractProjectModel* p, AbstractProjectModel::all() )
	{
		// check root project
		if ( p->filePath() == s )
		{
			mWorkspace->projectsManager()->setCurrentProject( p->rootProject() );
			return true;
		}
		// check subprojects
		foreach ( QModelIndex i, p->subProjects( p->rootProject(), true ) )
		{
			if ( p->filePath( i ) == s )
			{
				mWorkspace->projectsManager()->setCurrentProject( i );
				return true;
			}
		}
	}
	// create a new qmake project
	QMakeProjectModel* mProject = new QMakeProjectModel( s, this );
	// if we can open project
	if ( mProject->open() )
	{
		QMakeProjectProxy* mProxy = new QMakeProjectProxy( mProject );
		// add it to projects manager
		mWorkspace->projectsManager()->addProxy( mProxy );
		return true;
	}
	else
		delete mProject;
	return false;
}
//
Q_EXPORT_PLUGIN2( ProjectQMake, QMake )
