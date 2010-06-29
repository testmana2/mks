#include "PHPQtProjectItem.h"
#include "PluginsManager.h"
#include "XUPPlugin.h"

#include <XUPProjectItemInfos.h>
#include <pMonkeyStudio.h>
#include <BuilderPlugin.h>
#include <InterpreterPlugin.h>

#include <QApplication>
#include <QTextCodec>
#include <QFile>
#include <QDir>
#include <QFileInfo>

#include <QDebug>

int PHPQtProjectItem::projectType() const
{
	return PHPQtProjectItem::PHPQtProject;
}

void PHPQtProjectItem::registerProjectType() const
{
	// get proejct type
	int pType = projectType();

	// register it
	mXUPProjectInfos->unRegisterType( pType );
	mXUPProjectInfos->registerType( pType, const_cast<PHPQtProjectItem*>( this ) );

	// values
	const QString mPixmapsPath = ":/phpqtitems";
	const QStringList mOperators = QStringList( "=" );
	const QStringList mFilteredVariables = QStringList( "FORMS" ) << "PHP_FILES";
	const QStringList mFileVariables = QStringList( "FORMS" ) << "PHP_FILES";
	const QStringList mPathVariables = QStringList();
	const StringStringListList mSuffixes = StringStringListList()
		<< qMakePair( tr( "PHP-Qt Project" ), QStringList( "*.xphpqt" ) );
	const StringStringList mVariableLabels = StringStringList()
		<< qMakePair( QString( "FORMS" ), tr( "Qt Forms" ) )
		<< qMakePair( QString( "PHP_FILES" ), tr( "PHP Files" ) );
	const StringStringList mVariableIcons = StringStringList()
		<< qMakePair( QString( "FORMS" ), QString( "forms" ) )
		<< qMakePair( QString( "PHP_FILES" ), QString( "php" ) );

	// register values
	mXUPProjectInfos->registerPixmapsPath( pType, mPixmapsPath );
	mXUPProjectInfos->registerOperators( pType, mOperators );
	mXUPProjectInfos->registerFilteredVariables( pType, mFilteredVariables );
	mXUPProjectInfos->registerFileVariables( pType, mFileVariables );
	mXUPProjectInfos->registerPathVariables( pType, mPathVariables );
	mXUPProjectInfos->registerSuffixes( pType, mSuffixes );
	mXUPProjectInfos->registerVariableLabels( pType, mVariableLabels );
	mXUPProjectInfos->registerVariableIcons( pType, mVariableIcons );
}

XUPProjectItem* PHPQtProjectItem::newProject() const
{
	return new PHPQtProjectItem();
}

QStringList PHPQtProjectItem::autoActivatePlugins() const
{
	return QStringList("PHP");
}

void PHPQtProjectItem::installCommands()
{
	// get plugins
	InterpreterPlugin* ip = interpreter();
	
	// temp command
	pCommand cmd;

	// build command
	if ( ip )
	{
		cmd = ip->interpretCommand();
	}
	
	cmd.setUserData( QVariant::fromValue( &mCommands ) );
	cmd.setProject( this );
	cmd.setSkipOnError( false );
	const pCommand cmdInterpret = cmd;
	
	// get qt version
	QString mainFile = relativeFilePath( projectSettingsValue( "MAIN_FILE" ) );
	
	if ( mainFile.isEmpty() )
	{
		mainFile = relativeFilePath( findFile( "main.php" ).value( 0 ).absoluteFilePath() );
	}
	
	// available commands
	if ( ip )
	{
		// execute project
		cmd = cmdInterpret;
		cmd.setText( tr( "Start" ) );
		cmd.setArguments( mainFile );
		cmd.setParsers( QStringList() );
		cmd.setTryAllParsers( false );
		addCommand( cmd, "mInterpreter" );
		
	}
	
	// install defaults commands
	XUPProjectItem::installCommands();
}

QStringList PHPQtProjectItem::sourceFiles() const
{
	return mVariableCache.value( "PHP_FILES" ).split(" ");
}

void PHPQtProjectItem::addFiles( const QStringList& files, XUPItem* scope )
{
	XUPProjectItem* project = scope->project();
	
	foreach (QString file, files)
	{
		XUPItem* var = NULL;
		QString varName;
		if (file.endsWith(".ui"))
		{
			varName = "FORMS";
		}
		else if (file.endsWith(".php"))
		{
			varName = "PHP_FILES";
		}
		
		if (varName.isNull())
		{
			setLastError("Don't know how to add file " + file);
			return;
		}
		
		var = project->getVariable( scope, varName );
		if (NULL == var)
		{
			var = scope->addChild( XUPItem::Variable );
			var->setAttribute( "name", varName );
			
		}
		
		XUPItem* value = var->addChild( XUPItem::File );
		value->setAttribute( "content", project->relativeFilePath( file ) );
	}
}

XUPPlugin* PHPQtProjectItem::editorPlugin()
{
	return MonkeyCore::pluginsManager()->plugins<XUPPlugin*>( PluginsManager::stAll, "PHP-Qt" ).value( 0 );
}

InterpreterPlugin* PHPQtProjectItem::interpreter() const
{
	return MonkeyCore::pluginsManager()->plugin<InterpreterPlugin*>( PluginsManager::stAll, "PHP" );
}

StringStringListList PHPQtProjectItem::sourceFileNamePatterns() const
{
	return StringStringListList()
		<< qMakePair( QString( "Forms" ), QStringList( "*.ui" ) )
		<< qMakePair( QString( "PHP files" ), QStringList( "*.php*" ) );
}
