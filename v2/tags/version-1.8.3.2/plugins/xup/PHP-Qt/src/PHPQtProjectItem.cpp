#include "PHPQtProjectItem.h"

#include <XUPProjectItemInfos.h>
#include <pMonkeyStudio.h>
#include <QueuedStatusBar.h>
#include <BuilderPlugin.h>
#include <CompilerPlugin.h>
#include <InterpreterPlugin.h>

#include <QApplication>
#include <QTextCodec>
#include <QFile>
#include <QDir>
#include <QFileInfo>

#include <QDebug>

PHPQtProjectItem::PHPQtProjectItem()
	: XUPProjectItem()
{
}

PHPQtProjectItem::~PHPQtProjectItem()
{
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

	// Variable suffixes
	const StringStringListList mVariableSuffixes = StringStringListList()
		<< qMakePair( QString( "FORMS" ), QStringList( "*.ui" ) )
		<< qMakePair( QString( "PHP_FILES" ), QStringList( "*.php*" ) );

	// register values
	mXUPProjectInfos->registerPixmapsPath( pType, mPixmapsPath );
	mXUPProjectInfos->registerOperators( pType, mOperators );
	mXUPProjectInfos->registerFilteredVariables( pType, mFilteredVariables );
	mXUPProjectInfos->registerFileVariables( pType, mFileVariables );
	mXUPProjectInfos->registerPathVariables( pType, mPathVariables );
	mXUPProjectInfos->registerSuffixes( pType, mSuffixes );
	mXUPProjectInfos->registerVariableLabels( pType, mVariableLabels );
	mXUPProjectInfos->registerVariableIcons( pType, mVariableIcons );
	mXUPProjectInfos->registerVariableSuffixes( pType, mVariableSuffixes );
}

BuilderPlugin* PHPQtProjectItem::builder( const QString& plugin ) const
{
	return XUPProjectItem::builder( plugin );
}

CompilerPlugin* PHPQtProjectItem::compiler( const QString& plugin ) const
{
	return XUPProjectItem::compiler( plugin );
}

DebuggerPlugin* PHPQtProjectItem::debugger( const QString& plugin ) const
{
	return XUPProjectItem::debugger( plugin );
}

InterpreterPlugin* PHPQtProjectItem::interpreter( const QString& plugin ) const
{
	QString plug = plugin;

	if ( plug.isEmpty() )
	{
		plug = "PHP";
	}

	return XUPProjectItem::interpreter( plug );
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
		addCommand( cmd, "mInterpreter" );
		
	}
	
	// install defaults commands
	XUPProjectItem::installCommands();
}
