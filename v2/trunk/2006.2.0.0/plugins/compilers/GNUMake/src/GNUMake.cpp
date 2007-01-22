#include "GNUMake.h"
#include "AbstractProjectItemModel.h"
#include "MenuBar.h"
#include "ProjectsManager.h"
#include "PluginsManager.h"
#include "UISettings.h"
#include "Settings.h"
//
#include <QFileInfo>
#include <QFileDialog>
#include <QInputDialog>
//
//TODO  parentProject
GNUMake::~GNUMake()
{
	if ( isInstalled() )
		uninstall();
}
//
void GNUMake::initialize( Workspace* w )
{
	CompilerPlugin::initialize( w );
	//
	mPluginInfos.Caption = tr( "GNU Make Compiler" );
	mPluginInfos.Description = tr( "This plugin provides support of building and executing projects using the GNU Make Compiler" );
	mPluginInfos.Type = BasePlugin::iCompiler;
	mPluginInfos.Name = "GNU Make";
	mPluginInfos.Version = "0.1.0";
	mPluginInfos.Installed = false;
}
//
bool GNUMake::install()
{
	connect( mWorkspace->menuBar()->action( "mBuild/aCurrent" ), SIGNAL( triggered() ), this, SLOT( buildCurrent() ) );
	connect( mWorkspace->menuBar()->action( "mBuild/mBuild/aAll" ), SIGNAL( triggered() ), this, SLOT( buildAll() ) );
	connect( mWorkspace->menuBar()->action( "mBuild/mRebuild/aCurrent" ), SIGNAL( triggered() ), this, SLOT( reBuildCurrent() ) );
	connect( mWorkspace->menuBar()->action( "mBuild/mRebuild/aAll" ), SIGNAL( triggered() ), this, SLOT( reBuildAll() ) );
	connect( mWorkspace->menuBar()->action( "mBuild/aStop" ), SIGNAL( triggered() ), this, SLOT( stop() ) );
	connect( mWorkspace->menuBar()->action( "mBuild/mClean/aCurrent" ), SIGNAL( triggered() ), this, SLOT( cleanCurrent() ) );
	connect( mWorkspace->menuBar()->action( "mBuild/mClean/aAll" ), SIGNAL( triggered() ), this, SLOT( cleanAll() ) );
	connect( mWorkspace->menuBar()->action( "mBuild/mDistClean/aCurrent" ), SIGNAL( triggered() ), this, SLOT( distCleanCurrent() ) );
	connect( mWorkspace->menuBar()->action( "mBuild/mDistClean/aAll" ), SIGNAL( triggered() ), this, SLOT( distCleanAll() ) );
	connect( mWorkspace->menuBar()->action( "mBuild/aExecute" ), SIGNAL( triggered() ), this, SLOT( execute() ) );
	connect( mWorkspace->menuBar()->action( "mBuild/aExecuteWithParameters" ), SIGNAL( triggered() ), this, SLOT( executeWithParameters() ) );
	connect( mWorkspace->menuBar()->action( "mBuild/aDistCleanBuildExecute" ), SIGNAL( triggered() ), this, SLOT( distCleanBuildExecute() ) );
	// connect to console bridge
	Console* c = (Console*)mWorkspace->pluginsManager()->plugin( "Console" );
	if ( c )
	{
		connect( this, SIGNAL( messageBox( const QString& ) ), c, SIGNAL( messageBox( const QString& ) ) );
		connect( this, SIGNAL( clearMessageBox() ), c, SIGNAL( clearMessageBox() ) );
		connect( this, SIGNAL( runConsoleCommands( ConsoleCommands* ) ), c, SLOT( runConsoleCommands( ConsoleCommands* ) ) );
		connect( this, SIGNAL( stopConsole() ), c, SLOT( stopConsole() ) );
	}
	//
	mPluginInfos.Installed = true;
	return true;
}
//
bool GNUMake::uninstall()
{
	disconnect( mWorkspace->menuBar()->action( "mBuild/mBuild/aCurrent" ), SIGNAL( triggered() ), this, SLOT( buildCurrent() ) );
	disconnect( mWorkspace->menuBar()->action( "mBuild/mBuild/aAll" ), SIGNAL( triggered() ), this, SLOT( buildAll() ) );
	disconnect( mWorkspace->menuBar()->action( "mBuild/mRebuild/aCurrent" ), SIGNAL( triggered() ), this, SLOT( reBuildCurrent() ) );
	disconnect( mWorkspace->menuBar()->action( "mBuild/mRebuild/aAll" ), SIGNAL( triggered() ), this, SLOT( reBuildAll() ) );
	disconnect( mWorkspace->menuBar()->action( "mBuild/aStop" ), SIGNAL( triggered() ), this, SLOT( stop() ) );
	disconnect( mWorkspace->menuBar()->action( "mBuild/mClean/aCurrent" ), SIGNAL( triggered() ), this, SLOT( cleanCurrent() ) );
	disconnect( mWorkspace->menuBar()->action( "mBuild/mClean/aAll" ), SIGNAL( triggered() ), this, SLOT( cleanAll() ) );
	disconnect( mWorkspace->menuBar()->action( "mBuild/mDistClean/aCurrent" ), SIGNAL( triggered() ), this, SLOT( distCleanCurrent() ) );
	disconnect( mWorkspace->menuBar()->action( "mBuild/mDistClean/aAll" ), SIGNAL( triggered() ), this, SLOT( distCleanAll() ) );
	disconnect( mWorkspace->menuBar()->action( "mBuild/aExecute" ), SIGNAL( triggered() ), this, SLOT( execute() ) );
	disconnect( mWorkspace->menuBar()->action( "mBuild/aExecuteWithParameters" ), SIGNAL( triggered() ), this, SLOT( executeWithParameters() ) );
	disconnect( mWorkspace->menuBar()->action( "mBuild/aDistCleanBuildExecute" ), SIGNAL( triggered() ), this, SLOT( distCleanBuildExecute() ) );
	// disconnect from console bridge
	Console* c = (Console*)mWorkspace->pluginsManager()->plugin( "Console" );
	if ( c )
	{
		disconnect( this, SIGNAL( messageBox( const QString& ) ), c, SIGNAL( messageBox( const QString& ) ) );
		disconnect( this, SIGNAL( clearMessageBox() ), c, SIGNAL( clearMessageBox() ) );
		disconnect( this, SIGNAL( runConsoleCommands( ConsoleCommands* ) ), c, SLOT( runConsoleCommands( ConsoleCommands* ) ) );
		disconnect( this, SIGNAL( stopConsole() ), c, SLOT( stopConsole() ) );
	}
	//
	mPluginInfos.Installed = false;
	return true;
}
//
QWidget* GNUMake::settingsWidget()
{
	return new UISettings;
}
//
void GNUMake::buildCurrent()
{
	// got project
	AbstractProjectItemModel* p = currentProject();
	if ( !p )
		return;
	// create command sentences
	ConsoleCommands l;
	l << qmakeCommand( p );
	l << makeCommand( p );
	// send commands
	emit clearMessageBox();
	emit runConsoleCommands( &l );
}
//
void GNUMake::buildAll()
{
	// got project
	AbstractProjectItemModel* p = parentProject();
	if ( !p )
		return;
	// create command sentences
	ConsoleCommands l;
	l << qmakeCommand( p );
	l << makeCommand( p );
	// send commands
	emit clearMessageBox();
	emit runConsoleCommands( &l );
}
//
void GNUMake::reBuildCurrent()
{
	// got project
	AbstractProjectItemModel* p = currentProject();
	if ( !p )
		return;
	// create command sentences
	ConsoleCommands l;
	l << qmakeCommand( p );
	l << makeCleanCommand( p );
	l << makeCommand( p );
	// send commands
	emit clearMessageBox();
	emit runConsoleCommands( &l );
};
//
void GNUMake::reBuildAll()
{
	// got project
	AbstractProjectItemModel* p = parentProject();
	if ( !p )
		return;
	// create command sentences
	ConsoleCommands l;
	l << qmakeCommand( p );
	l << makeCleanCommand( p );
	l << makeCommand( p );
	// send commands
	emit clearMessageBox();
	emit runConsoleCommands( &l );
}
//
void GNUMake::stop()
{
	emit stopConsole();
};
//
void GNUMake::cleanCurrent()
{
	// got project
	AbstractProjectItemModel* p = currentProject();
	if ( !p )
		return;
	// create command sentences
	ConsoleCommands l;
	l << qmakeCommand( p );
	l << makeCleanCommand( p );
	// send commands
	emit clearMessageBox();
	emit runConsoleCommands( &l );
};
//
void GNUMake::cleanAll()
{
	// got project
	AbstractProjectItemModel* p = parentProject();
	if ( !p )
		return;
	// create command sentences
	ConsoleCommands l;
	l << qmakeCommand( p );
	l << makeCleanCommand( p );
	// send commands
	emit clearMessageBox();
	emit runConsoleCommands( &l );
}
//
void GNUMake::distCleanCurrent()
{
	// got project
	AbstractProjectItemModel* p = currentProject();
	if ( !p )
		return;
	// create command sentences
	ConsoleCommands l;
	l << qmakeCommand( p );
	l << makeDistCleanCommand( p );
	// send commands
	emit clearMessageBox();
	emit runConsoleCommands( &l );
};
//
void GNUMake::distCleanAll()
{
	// got project
	AbstractProjectItemModel* p = parentProject();
	if ( !p )
		return;
	// create command sentences
	ConsoleCommands l;
	l << qmakeCommand( p );
	l << makeDistCleanCommand( p );
	// send commands
	emit clearMessageBox();
	emit runConsoleCommands( &l );
}
//
void GNUMake::execute() 
{
	// got project
	AbstractProjectItemModel* p = currentProject();
	if ( !p )
		return;
	// create command sentences
	ConsoleCommands l;
	l << executeCommand( p );
	// send commands
	emit clearMessageBox();
	emit runConsoleCommands( &l );
};
//
void GNUMake::executeWithParameters() 
{
	// got project
	AbstractProjectItemModel* p = currentProject();
	if ( !p )
		return;
	// get params
	bool b;
	QString s;
	s = QInputDialog::getText( qApp->activeWindow(), tr( "Parameters" ), tr( "Input parameters for executing " ), QLineEdit::Normal, s, &b );
	// create command sentences
	ConsoleCommands l;
	l << executeCommand( p, b ? s : QString::null );
	// send commands
	emit clearMessageBox();
	emit runConsoleCommands( &l );
};
//
void GNUMake::distCleanBuildExecute()
{
	// got project
	AbstractProjectItemModel* p = currentProject();
	if ( !p )
		return;
	// create command sentences
	ConsoleCommands l;
	l << qmakeCommand( p );
	l << makeDistCleanCommand( p );
	l << qmakeCommand( p );
	l << makeCommand( p );
	l << executeCommand( p );
	// send commands
	emit clearMessageBox();
	emit runConsoleCommands( &l );
}
//
AbstractProjectItemModel* GNUMake::currentProject()
{
	return mWorkspace->projectsManager()->currentProject();
}
//
AbstractProjectItemModel* GNUMake::parentProject()
{
	// need fix to got real parent
	return currentProject();
}
//
ConsoleCommand GNUMake::qmakeCommand( AbstractProjectItemModel* p ) const
{
	return ConsoleCommand( "qmake-qt4", p->path() );
}
//
ConsoleCommand GNUMake::makeCommand( AbstractProjectItemModel* p ) const
{
	// if there is a path we need sure it s added to system environment once
	if ( !Settings::current()->value( "Plugins/GNU Make/Path" ).toString().isEmpty() )
	{
		// got the console bridge
		Console* c = (Console*)mWorkspace->pluginsManager()->plugin( "Console" );
		if ( c )
		{
			// got environment variables
			QStringList l = c->systemEnvironment();
			QString p = Settings::current()->value( "Plugins/GNU Make/Path" ).toString();
			if ( p.endsWith( "/" ) )
				p.chop( 1 );
			// add make tool path
			int i = 0;
			QString s = ":";
#if defined ( Q_WS_WIN )
			s = ";";
#endif
			while( ( i = l.indexOf( QRegExp( "^PATH=(.*)" ) ) ) != -1 )
			{
				if ( !l.at( i ).contains( p, Qt::CaseInsensitive ) )
				{
					l[i].append( s +p );
					i++;
				}
			}
			c->setSystemEnvironment( l );
		}
	}
	QString s = Settings::current()->value( "Plugins/GNU Make/Binary" ).toString();
	if ( !s.isNull() )
		return ConsoleCommand( s, p->path() );
	return ConsoleCommand( QString::null, QString::null ); // this command will not be executed by console
}
//
ConsoleCommand GNUMake::makeCleanCommand( AbstractProjectItemModel* p ) const
{
	ConsoleCommand c = makeCommand( p );
	if ( !c.isNull() )
		c.setCommand( c.command().append( " clean" ) );
	return c;
}
//
ConsoleCommand GNUMake::makeDistCleanCommand( AbstractProjectItemModel* p ) const
{
	ConsoleCommand c = makeCommand( p );
	if ( !c.isNull() )
		c.setCommand( c.command().append( " distclean" ) );
	return c;
}
//
ConsoleCommand GNUMake::executeCommand( AbstractProjectItemModel* p, const QString& s ) const
{
	// get binary path
	QString mPath = p->getValue( "DESTDIR" );
	if ( mPath.isEmpty() )
	{
		QString pConfig = p->getValues( "CONFIG" ).toLower();
		mPath = p->path();
#if defined ( Q_WS_WIN )
		if ( pConfig.contains( "debug" ) )
			mPath.append( "/debug" );
		else if ( pConfig.contains( "release" ) )
			mPath.append( "/release" );
		else
			mPath.append( "/debug" ); // need check i'm not sure
#elif defined ( Q_WS_MAC )
		// todo need help
#elif defined ( Q_WS_QWS )
		// todo need help
#elif defined ( Q_WS_X11 )
		// nothing to do
#endif
	}
	// get binary name
	QString mFile = p->getValue( "TARGET" );
	if ( !p->getValue( "TEMPLATE" ).toLower().contains( "app" ) )
		mFile.clear();
	else if ( mFile.isEmpty() )
		mFile = p->name();
	// check file exist
	QFileInfo f( QString( "%1/%2" ).arg( mPath, mFile ) );
	QString mFilePath = f.canonicalFilePath();
	if ( f.isDir() || !f.exists() )
		mFilePath = QFileDialog::getOpenFileName( qApp->activeWindow(), tr( "Choose the file to be executed" ), p->path() );
	f.setFile( mFilePath );
	if ( f.exists() )
	{
		mFile = f.fileName();
#if defined ( Q_WS_WIN )
		// nothing to do
#elif defined ( Q_WS_MAC )
		// may need fix by a mac user
#elif defined ( Q_WS_QWS )
		// todo need help
#elif defined ( Q_WS_X11 )
		mFile.prepend( "./" );
#endif
		if ( !s.isEmpty() )
			mFile.append( QString( " %1" ).arg( s ) );
		return ConsoleCommand( mFile, f.path() );
	}
	return ConsoleCommand( QString::null, QString::null ); // this command will not be executed by console
}
//
Q_EXPORT_PLUGIN2( CompilerGNUMake, GNUMake )
